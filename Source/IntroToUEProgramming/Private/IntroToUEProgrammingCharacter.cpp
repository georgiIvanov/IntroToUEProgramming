// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "IntroToUEProgramming.h"
#include "IntroToUEProgrammingCharacter.h"
#include "BatteryPickup.h"
#include "IntroToUEProgrammingGameMode.h"

//////////////////////////////////////////////////////////////////////////
// AIntroToUEProgrammingCharacter

AIntroToUEProgrammingCharacter::AIntroToUEProgrammingCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    PowerLevel = 2000.f;
    SpeedFactor = 0.75f;
    BaseSpeed = 300.0f;
    
    //Create our battery collection volume
    CollectionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollectionSphere"));
    CollectionSphere->AttachTo(RootComponent);
    CollectionSphere->SetSphereRadius(200.f);
    
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AIntroToUEProgrammingCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    InputComponent->BindAction("CollectPickups", IE_Pressed, this, &AIntroToUEProgrammingCharacter::CollectBatteries);

	InputComponent->BindAxis("MoveForward", this, &AIntroToUEProgrammingCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AIntroToUEProgrammingCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AIntroToUEProgrammingCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AIntroToUEProgrammingCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AIntroToUEProgrammingCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AIntroToUEProgrammingCharacter::TouchStopped);
}


void AIntroToUEProgrammingCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AIntroToUEProgrammingCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AIntroToUEProgrammingCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIntroToUEProgrammingCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AIntroToUEProgrammingCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AIntroToUEProgrammingCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AIntroToUEProgrammingCharacter::CollectBatteries()
{
    float BatteryPower = 0.f;
    
    AIntroToUEProgrammingGameMode* GameMode = Cast<AIntroToUEProgrammingGameMode>(UGameplayStatics::GetGameMode(this));
    
    // if we are not playing we can't collect batteries
    if(GameMode->GetCurrentState() != EPlayState::EPlaying)
    {
        return;
    }
    
    // Get all overlapping Actors and store them in a CollectedActors array
    TArray<AActor*> CollectedActors;
    CollectionSphere->GetOverlappingActors(CollectedActors);
    
    // for each actor collected
    for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
    {
        // Cast the collected Actor to ABatteryPickup
        ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(CollectedActors[iCollected]);
        
        // if successful and battery is active
        if(TestBattery &&
           !TestBattery->IsPendingKill() &&
           TestBattery->bIsActive)
        {
            BatteryPower = BatteryPower + TestBattery->PowerLevel;
            TestBattery->OnPickedUp();
            TestBattery->bIsActive = false;
        }
    }
    
    if(BatteryPower > 0)
    {
        // Call the Blueprint implementation of PowerUp with
        // the total battery power as input
        PowerUp(BatteryPower);
    }
}

void AIntroToUEProgrammingCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    GetCharacterMovement()->MaxWalkSpeed = SpeedFactor * PowerLevel + BaseSpeed;
}

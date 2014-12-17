// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "IntroToUEProgramming.h"
#include "IntroToUEProgrammingGameMode.h"
#include "IntroToUEProgrammingCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TutorialHud.h"

AIntroToUEProgrammingGameMode::AIntroToUEProgrammingGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    
    // set default hud class
    HUDClass = ATutorialHud::StaticClass();
    
    // base decay rate
    DecayRate = 1.f;
    
}

void AIntroToUEProgrammingGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // find all spawn volume actors
    TArray<AActor*> FoundActors;
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
    
    for(auto Actor : FoundActors)
    {
        ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
        if(SpawnVolumeActor)
        {
            SpawnVolumeActors.Add(SpawnVolumeActor);
        }
    }
    
    SetCurrentState(EPlayState::EPlaying);
}

void AIntroToUEProgrammingGameMode::Tick(float DeltaSeconds)
{
    AIntroToUEProgrammingCharacter* MyCharacter = Cast<AIntroToUEProgrammingCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    
    if(MyCharacter->PowerLevel > 0.5)
    {
        MyCharacter->PowerLevel = FMath::FInterpTo(MyCharacter->PowerLevel, 0.f, DeltaSeconds, DecayRate);
    }
    else
    {
        SetCurrentState(EPlayState::EGameOver);
    }
    
}

void AIntroToUEProgrammingGameMode::SetCurrentState(EPlayState NewState)
{
    CurrentState = NewState;
    
    HandleNewState(NewState);
}

void AIntroToUEProgrammingGameMode::HandleNewState(EPlayState NewState)
{
    switch (NewState) {
        case EPlayState::EPlaying:
        {
            SpawnVolumesActive(true);
        }
            break;
            
        case EPlayState::EGameOver:
        {
            SpawnVolumesActive(false);
            
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            PlayerController->SetCinematicMode(true, true, true);
        }
            break;
            
        case EPlayState::EUnknown:
        {
            
        }
        default:
            break;
    }
}

void AIntroToUEProgrammingGameMode::SpawnVolumesActive(bool setActive)
{
    for(ASpawnVolume* Volume : SpawnVolumeActors)
    {
        Volume->EnableSpawning(setActive);
    }
}


// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "IntroToUEProgramming.h"
#include "IntroToUEProgrammingGameMode.h"
#include "IntroToUEProgrammingCharacter.h"
#include "Kismet/GameplayStatics.h"

AIntroToUEProgrammingGameMode::AIntroToUEProgrammingGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    
    // base decay rate
    DecayRate = 1.f;
    
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
            
            break;
        case EPlayState::EGameOver:
            
            break;
        case EPlayState::EUnknown:
            
        default:
            break;
    }
}


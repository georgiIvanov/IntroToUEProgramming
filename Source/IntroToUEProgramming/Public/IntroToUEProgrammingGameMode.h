// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "IntroToUEProgrammingGameMode.generated.h"

// enum for current state of gameplay
enum class EPlayState : short
{
    EPlaying,
    EGameOver,
    EUnknown
};

UCLASS(minimalapi)
class AIntroToUEProgrammingGameMode : public AGameMode
{
	GENERATED_BODY()
    
    EPlayState CurrentState;

    void HandleNewState(EPlayState NewState);
    
public:
	AIntroToUEProgrammingGameMode(const FObjectInitializer& ObjectInitializer);
    
    virtual void Tick(float DeltaSeconds) OVERRIDE;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
    float DecayRate;
    
    EPlayState GetCurrentState() const;
    
    void SetCurrentState(EPlayState NewState);
};

FORCEINLINE EPlayState AIntroToUEProgrammingGameMode::GetCurrentState() const
{
    return CurrentState;
}


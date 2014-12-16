// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SpawnVolume.h"
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
    TArray<ASpawnVolume*> SpawnVolumeActors;

    void HandleNewState(EPlayState NewState);
    void SpawnVolumesActive(bool setActive);
    
public:
	AIntroToUEProgrammingGameMode(const FObjectInitializer& ObjectInitializer);
    
    virtual void Tick(float DeltaSeconds) OVERRIDE;
    virtual void BeginPlay() OVERRIDE;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
    float DecayRate;
    
    EPlayState GetCurrentState() const;
    
    void SetCurrentState(EPlayState NewState);
};

FORCEINLINE EPlayState AIntroToUEProgrammingGameMode::GetCurrentState() const
{
    return CurrentState;
}


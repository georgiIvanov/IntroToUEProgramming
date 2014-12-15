// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "SpawnVolume.generated.h"

/**
 * 
 */
UCLASS()
class INTROTOUEPROGRAMMING_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
    
    
    // calculates random spawn delay
    float GetRandomSpawnDelay();
    
    // handles spawning of a new pickup
    void SpawnPickup();
    
    // current spawn delay
    float SpawnDelay;
    
    // timer for when to spawn the pickup
    float SpawnTime;
	
public:
    
    ASpawnVolume(const FObjectInitializer& ObjectInitializer);
    
    // box component to specify the spawning area within the level
    UPROPERTY(VisibleInstanceOnly, Category = Spawning)
    class UBoxComponent* WhereToSpawn;
	
    // pickup to spawn
    UPROPERTY(EditAnywhere, Category = Spawning)
    TSubclassOf<class APickup> WhatToSpawn;
    
    // minimum spawn delay
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    float SpawnDelayRangeLow;
    
    // maximum spawn delay
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
    float SpawnDelayRangeHigh;
    
    // random point within the box component
    UFUNCTION(BlueprintPure, Category = Spawning)
    FVector GetRandomPointInVolume();
    
    virtual void Tick(float DeltaSeconds) OVERRIDE;
};

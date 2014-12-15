// Fill out your copyright notice in the Description page of Project Settings.

#include "IntroToUEProgramming.h"
#include "SpawnVolume.h"
#include "BatteryPickup.h"

ASpawnVolume::ASpawnVolume(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // create simple StaticMeshComponent to represent
    // the pickup in the level
    WhereToSpawn = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WhereToSpawn"));
    
    // set static mesh component as the root component
    RootComponent = WhereToSpawn;
    
    // set the spawn delay range
    SpawnDelayRangeLow = 1.0f;
    SpawnDelayRangeHigh = 4.5f;
    SpawnDelay = GetRandomSpawnDelay();
    
    // make spawn volume tickable
    PrimaryActorTick.bCanEverTick = true;
}

void ASpawnVolume::SpawnPickup()
{
    // if we have set something to spawn
    if(WhatToSpawn != NULL)
    {
        // check for a valid world
        UWorld* const World = GetWorld();
        if(World)
        {
            // set the spawn parameters
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            
            // get random location to spawn at
            FVector SpawnLocation = GetRandomPointInVolume();
            
            // get random rotation for the spawned item
            FRotator SpawnRotation;
            SpawnRotation.Yaw = FMath::FRand() * 360.f;
            SpawnRotation.Pitch = FMath::FRand() * 360.f;
            SpawnRotation.Roll = FMath::FRand() * 360.f;
            
            // spawn the pickup
            APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
            
        }
    }
}

float ASpawnVolume::GetRandomSpawnDelay()
{
    // get random float within the spawn delay range
    return FMath::RandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
}

FVector ASpawnVolume::GetRandomPointInVolume()
{
    FVector RandomLocation;
    float MinX, MinY, MinZ;
    float MaxX, MaxY, MaxZ;
    
    FVector Origin;
    FVector BoxExtent;
    
    // get the spawnvolume's origin and box extent
    Origin = WhereToSpawn->Bounds.Origin;
    BoxExtent = WhereToSpawn->Bounds.BoxExtent;
    
    // calculate the minimum x, y and z
    MinX = Origin.X - BoxExtent.X / 2.f;
    MinY = Origin.Y - BoxExtent.Y / 2.f;
    MinZ = Origin.Z - BoxExtent.Z / 2.f;
    
    // calculate the maximum x, y and z
    MaxX = Origin.X + BoxExtent.X / 2.f;
    MaxY = Origin.Y + BoxExtent.Y / 2.f;
    MaxZ = Origin.Z + BoxExtent.Z / 2.f;
    
    // random spawn location will fall between min and max X, Y and Z
    RandomLocation.X = FMath::FRandRange(MinX, MaxX);
    RandomLocation.Y = FMath::FRandRange(MinY, MaxY);
    RandomLocation.Z = FMath::FRandRange(MinZ, MaxZ);
    
    return RandomLocation;
}


void ASpawnVolume::Tick(float DeltaSeconds)
{
    // always add delta seconds to spawn time
    SpawnTime += DeltaSeconds;
    
    bool bShouldSpawn = (SpawnTime > SpawnDelay);
    
    if(bShouldSpawn)
    {
        SpawnPickup();
        
        // deduct spawn delay from accumulated time value
        SpawnTime -= SpawnDelay;
        SpawnDelay = GetRandomSpawnDelay();
    }
}


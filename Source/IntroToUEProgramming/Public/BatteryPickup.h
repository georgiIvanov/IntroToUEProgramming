// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class INTROTOUEPROGRAMMING_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
	
public:
    ABatteryPickup(const FObjectInitializer& ObjectInitializer);
    
    // Amount of power battery gives to the player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
    float PowerLevel;
	
    // Override the OnPickedUp(use _Implementation because
    // this is a BlueprintNativeEvent)
    void OnPickedUp_Implementation() OVERRIDE;
};

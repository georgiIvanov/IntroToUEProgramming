// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class INTROTOUEPROGRAMMING_API APickup : public AActor
{
	GENERATED_BODY()

public:
    APickup(const FObjectInitializer& ObjectInitializer);
    
    // True if the pickup is able to be picked up, false if its deactivated
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
    bool bIsActive;
    
    // Simple collision primitive to use as root component
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
    class USphereComponent* BaseCollisionComponent;
    
    // Represents the pickup in the level
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
    class UStaticMeshComponent* PickupMesh;
    
    // Function to call when the pickup is collected
    UFUNCTION(BlueprintNativeEvent)
    void OnPickedUp();
    
};

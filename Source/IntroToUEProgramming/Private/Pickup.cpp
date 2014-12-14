// Fill out your copyright notice in the Description page of Project Settings.

#include "IntroToUEProgramming.h"
#include "Pickup.h"


APickup::APickup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    bIsActive = true;
    
    // create SphereComponent to handle pickup's collision
    BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSpereComponent"));
    
    // set sphere component as root component
    RootComponent = BaseCollisionComponent;
    
    // create static mesh component
    PickupMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));
    
    // turn physics on for static mesh
    PickupMesh->SetSimulatePhysics(true);
    
    // attach static mesh component to root component
    PickupMesh->AttachTo(RootComponent);
}

void APickup::OnPickedUp_Implementation()
{
    // no default implementation
}

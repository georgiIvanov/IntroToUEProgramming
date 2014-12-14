// Fill out your copyright notice in the Description page of Project Settings.

#include "IntroToUEProgramming.h"
#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // default powerlevel of battery
    PowerLevel = 150;
    
    
}

void ABatteryPickup::OnPickedUp_Implementation()
{
    Super::OnPickedUp_Implementation();
    
    // destroy battery
    Destroy();
}

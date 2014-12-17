// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TutorialHud.generated.h"

/**
 * 
 */
UCLASS()
class INTROTOUEPROGRAMMING_API ATutorialHud : public AHUD
{
	GENERATED_BODY()
	
public:
    
    ATutorialHud(const FObjectInitializer& ObjectInitializer);
    
    // variable for storing the font
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UFont* HUDFont;
	
    // primary draw call for the HUD
    virtual void DrawHUD() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "IntroToUEProgramming.h"
#include "TutorialHud.h"
#include "IntroToUEProgrammingGameMode.h"
#include "IntroToUEProgrammingCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

ATutorialHud::ATutorialHud(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // use the RobotoDistanceField font from the engine
    static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
    HUDFont = HUDFontOb.Object;
}

void ATutorialHud::DrawHUD()
{
    // get screen dimensions
    FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
    
    // call to the parent versions of DrawHUD
    Super::DrawHUD();
    
    // get the character and print its power level
    AIntroToUEProgrammingCharacter* MyCharacter = Cast<AIntroToUEProgrammingCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    FString PowerLevelString = FString::Printf(TEXT("%10.1f"), FMath::Abs(MyCharacter->PowerLevel));
    DrawText(PowerLevelString, FColor::White, 50, 50, HUDFont);
    
    AIntroToUEProgrammingGameMode* MyGameMode = Cast<AIntroToUEProgrammingGameMode>(UGameplayStatics::GetGameMode(this));
    
    
    // if the game is over
    if(MyGameMode->GetCurrentState() == EPlayState::EGameOver)
    {
        // create a variable for storing the size of printing game over
        FVector2D GameOverSize;
        GetTextSize(TEXT("GAME OVER"), GameOverSize.X, GameOverSize.Y, HUDFont);
        DrawText(TEXT("GAME OVER"), FColor::White, (ScreenDimensions.X - GameOverSize.X) / 2.f, (ScreenDimensions.Y - GameOverSize.Y) / 2.f, HUDFont);
    }
}


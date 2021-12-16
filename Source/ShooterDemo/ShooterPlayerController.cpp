// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"

void AShooterPlayerController::GameHasEnded(class AActor * EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    UUserWidget *Widget = CreateWidget(this, LoseScreen);
    if (Widget)
    {
        APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AShooterPlayerController::SetPausing, 1, false);
        }
        Widget->AddToViewport();
    }
}
	
void AShooterPlayerController::SetPausing()
{
    SetPause(true);
}

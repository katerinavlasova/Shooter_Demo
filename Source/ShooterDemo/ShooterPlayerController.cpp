// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"

void AShooterPlayerController::GameHasEnded(class AActor * EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    UE_LOG(LogTemp, Warning, TEXT("gamemode!!!!!!!!!!!!!!!"));
    UUserWidget *Widget = CreateWidget(this, LoseScreen);
    if (Widget)
    {
        APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            FTimerHandle TimerHandle;
            //FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AShooterPlayerController::SetPause, true);
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AShooterPlayerController::SetPausing, 1, false);
            //GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            //SetPause(true);
        }
        Widget->AddToViewport();
    }
}
	
void AShooterPlayerController::SetPausing()
{
    SetPause(true);
}


           // FTimerHandle TimerHandle;
            // FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &APlayerController::SetPause, true);
            // GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 2, false);
            // GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            //PlayerController->
            //SetPause(true);
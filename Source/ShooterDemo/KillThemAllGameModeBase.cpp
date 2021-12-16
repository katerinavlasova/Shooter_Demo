// Fill out your copyright notice in the Description page of Project Settings.


#include "KillThemAllGameModeBase.h"

void AKillThemAllGameModeBase::PawnKilled(APawn *KilledPawn)
{
    Super::PawnKilled(KilledPawn);
    APlayerController *PlayerController = Cast<APlayerController>(KilledPawn->GetController());
    if (PlayerController)
    {
        PlayerController->GameHasEnded(nullptr, false);
    }
}

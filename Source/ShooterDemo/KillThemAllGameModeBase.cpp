// Fill out your copyright notice in the Description page of Project Settings.


#include "KillThemAllGameModeBase.h"

void AKillThemAllGameModeBase::PawnKilled(APawn *KilledPawn)
{
    Super::PawnKilled(KilledPawn);
    UE_LOG(LogTemp, Error, TEXT("smb is dead.."));
    APlayerController *PlayerController = Cast<APlayerController>(KilledPawn->GetController());
    if (PlayerController)
    {
        PlayerController->GameHasEnded(nullptr, false);
    }
}

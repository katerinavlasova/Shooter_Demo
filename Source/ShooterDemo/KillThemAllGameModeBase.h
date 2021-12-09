// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterDemoGameModeBase.h"
#include "KillThemAllGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API AKillThemAllGameModeBase : public AShooterDemoGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn *KilledPawn) override;
};

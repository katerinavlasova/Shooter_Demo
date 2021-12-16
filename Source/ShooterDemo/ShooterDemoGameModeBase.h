// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API AShooterDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	//basic implementation to override in child class
	virtual void PawnKilled(APawn *KilledPawn);
};

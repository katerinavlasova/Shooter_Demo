// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"

#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor * EndGameFocus = nullptr, bool bIsWinner = false) override;
	UFUNCTION()
	void SetPausing();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreen;
};

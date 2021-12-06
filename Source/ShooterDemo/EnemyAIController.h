// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Properties", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree *BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Properties", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent *BlackboardComponent;

};

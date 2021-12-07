// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API UBTService_PlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

private:
	UBTService_PlayerLocation();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

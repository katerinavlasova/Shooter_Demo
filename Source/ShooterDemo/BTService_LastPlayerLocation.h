// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_LastPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API UBTService_Blackboard : public UBTService_BlackboardBase
{
	GENERATED_BODY()

private:
	UBTService_Blackboard();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

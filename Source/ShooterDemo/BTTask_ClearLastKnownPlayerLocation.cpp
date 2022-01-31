// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearLastKnownPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Blackboard::UBTTask_Blackboard()
{
    NodeName = TEXT("Clear Bkackboard Value");
}


EBTNodeResult::Type UBTTask_Blackboard::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("LastKnownPlayerLocation"));
    return EBTNodeResult::Succeeded;
}

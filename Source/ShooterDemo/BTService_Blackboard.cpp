// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Blackboard.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_Blackboard::UBTService_Blackboard()
{
    NodeName = "UpdateLastKnownPlayerLocation";
}

void UBTService_Blackboard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr)
    {
        return;
    }
    OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}
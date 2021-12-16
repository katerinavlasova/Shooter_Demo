// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterCharacter.h"



EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    APawn *ActorPawn = OwnerComp.GetAIOwner()->GetPawn();
    AShooterCharacter *Character = Cast<AShooterCharacter>(ActorPawn);
    if (Character)
    {
        Character->Shoot();
    }
    return EBTNodeResult::Succeeded;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"



AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (BehaviorTree != nullptr)
    {
        RunBehaviorTree(BehaviorTree);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("PatrollnitLocation"), this->GetPawn()->GetActorLocation());
    }
    //SetFocus(PlayerPawn);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (BehaviorTree)
    {
        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (LineOfSightTo(PlayerPawn))
        {
            GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
        }
        else
        {
            GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
        }

    }
    // APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // if (PlayerPawn)
    // {   
    //     if (this->LineOfSightTo(PlayerPawn))
    //     {
    //         MoveToActor(PlayerPawn, 20);
    //     }
    // }
}

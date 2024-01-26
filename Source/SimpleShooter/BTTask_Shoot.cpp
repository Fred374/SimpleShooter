// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner())
    {

        AShooterCharacter *OwnerCharacter = (AShooterCharacter *)OwnerComp.GetAIOwner()->GetPawn();
        if (OwnerCharacter)
        {
            OwnerCharacter->PullTrigger();
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
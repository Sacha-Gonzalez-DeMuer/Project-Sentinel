// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_MeleeAttack.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_TakeCover.h"

UBTTask_TakeCover::UBTTask_TakeCover()
{
}

void UBTTask_TakeCover::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
}

EBTNodeResult::Type UBTTask_TakeCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

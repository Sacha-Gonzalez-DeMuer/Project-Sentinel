// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_ReviveSentinel.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTTask_ReviveSentinel::UBTTask_ReviveSentinel()
{
}

EBTNodeResult::Type UBTTask_ReviveSentinel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ASentinelCharacter* Patient = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SentinelToRevive.SelectedKeyName)))
	{
		ToRevive = Patient;
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;;
}

void UBTTask_ReviveSentinel::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if(ToRevive->IsOnLastStand())
	{
		
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	
}

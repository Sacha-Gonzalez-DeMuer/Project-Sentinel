// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_SetRole.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UBTTask_SetRole::UBTTask_SetRole()
{
	NodeName = "Set Role";
}

EBTNodeResult::Type UBTTask_SetRole::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(SentinelTargetKey.SelectedKeyName));

	Sentinel->GetSentinelController()->SetRole(SetToRole);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

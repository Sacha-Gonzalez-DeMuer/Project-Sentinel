// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorTasks/BTTask_RequestAttackers.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTTask_RequestAttackers::UBTTask_RequestAttackers()
{
	NodeName = TEXT("RequestAttackers");
}

EBTNodeResult::Type UBTTask_RequestAttackers::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT(" request escort"))

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::Squad)));
	ASentinelCharacter* ToKill = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(SentinelToAttackKey.SelectedKeyName));
	if(Squad && ToKill && Squad->RequestEscort(ToKill))
	{
		return  EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed request escort :("))
	return  EBTNodeResult::Failed;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_LookAt.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"


UBTTask_LookAt::UBTTask_LookAt()
{
	NodeName = "Look At";
}

EBTNodeResult::Type UBTTask_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCBase* NPCBase = Cast<ANPCBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(BBKeys::SelfActor)));
	const ASentinelCharacter* Target = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(LookatTarget.SelectedKeyName));

	if(!NPCBase || !Target)
		return EBTNodeResult::Failed;
	
	NPCBase->SetActorRotation((Target->GetActorLocation() - NPCBase->GetActorLocation()).Rotation());

	return EBTNodeResult::Succeeded;
}

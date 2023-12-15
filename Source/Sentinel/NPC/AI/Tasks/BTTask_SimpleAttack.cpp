// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_SimpleAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

class ANPCBase;

UBTTask_SimpleAttack::UBTTask_SimpleAttack()
	: Damage(10.0f)
{
	NodeName = TEXT("Simple Attack");
}

EBTNodeResult::Type UBTTask_SimpleAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("Simple attack!"));

	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (const ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName)))
	{
		ANPCBase* NPC = Cast<ANPCBase>(Blackboard->GetValueAsObject(FName(BBKeys::SelfActor)));
		NPC->Attack(Sentinel, Damage);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
   
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}

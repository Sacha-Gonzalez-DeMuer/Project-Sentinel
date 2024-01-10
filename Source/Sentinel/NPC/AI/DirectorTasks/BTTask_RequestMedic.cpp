

#include "Sentinel/NPC/AI/DirectorTasks/BTTask_RequestMedic.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UBTTask_RequestMedic::UBTTask_RequestMedic()
{
	NodeName = "Request Medics";
}

EBTNodeResult::Type UBTTask_RequestMedic::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT(" request medic"))

	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::SelfActor)));
	ASentinelCharacter* ToHeal = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(SentinelToHealKey.SelectedKeyName));

	int MedicsInSquad = 0;

	for(ASentinelCharacter* Sentinel : Squad->GetSentinels())
	{
		if(Sentinel->GetSentinelController()->GetRole() == ERoles::Medic)
			++MedicsInSquad;
	}

	if(Squad && ToHeal && Squad->RequestMedic(ToHeal))
	{
		return  EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed request medic :("))
	return  EBTNodeResult::Failed;
}

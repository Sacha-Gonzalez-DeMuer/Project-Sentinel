// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorDecorators/BTDecorator_HasDefendersThanThreats.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTDecorator_HasDefendersThanThreats::UBTDecorator_HasDefendersThanThreats()
{
}

bool UBTDecorator_HasDefendersThanThreats::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                 uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::Squad)));

	const int SquadSize = Squad->GetNrSentinels();

	ASentinelCharacter* ThreatenedSentinel = Cast<ASentinelCharacter>(
		Blackboard->GetValueAsObject(SentinelWithThreats.SelectedKeyName));
	
	if(!ThreatenedSentinel) return false;
	const int NrAttackers = ThreatenedSentinel->GetTargetedBySet().Num();

	if(CheckForLess)
		return SquadSize < NrAttackers;
	else return SquadSize > NrAttackers;
}

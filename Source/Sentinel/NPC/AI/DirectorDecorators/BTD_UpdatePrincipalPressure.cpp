// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorDecorators/BTD_UpdatePrincipalPressure.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"

class ASentinelController;

UBTD_UpdatePrincipalPressure::UBTD_UpdatePrincipalPressure()
{
	
}

bool UBTD_UpdatePrincipalPressure::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	//// Get the blackboard component
	//UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//if (!BlackboardComp)
	//{
	//	// Handle the absence of the blackboard component (you might want to log an error or handle it differently)
	//	UE_LOG(LogTemp, Warning, TEXT("[UBTD_UpdatePrincipalPressure::CalculateRawConditionValue] Blackboard not found."));
	//	return false;
	//}
//
	//ASentinelCharacter* Principal = Cast<ASentinelCharacter>(BlackboardComp->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));
	//if(!Principal)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("[UBTD_UpdatePrincipalPressure::CalculateRawConditionValue] Squad has no principal"));
	//	return false;
	//}
	//
	//// Implement your logic to calculate the PressureToPrincipal value here
	//const float CalculatedPressure = CalculatePressure(Principal, BlackboardComp);
//
	//UE_LOG(LogTemp, Log, TEXT("[UBTD_UpdatePrincipalPressure] Pressure on principal: %f"), CalculatedPressure);
	//
	//// Update the "PressureToPrincipal" key in the blackboard
	//BlackboardComp->SetValueAsFloat(GetSelectedBlackboardKey(), CalculatedPressure);
//
	//// Return true or false based on your condition
	return true;
}
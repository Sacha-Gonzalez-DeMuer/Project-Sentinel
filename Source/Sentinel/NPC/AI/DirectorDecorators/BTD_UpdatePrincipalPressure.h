// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_UpdatePrincipalPressure.generated.h"

class ASentinelCharacter;
/**
 * 
 */
UCLASS()
class SENTINEL_API UBTD_UpdatePrincipalPressure : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTD_UpdatePrincipalPressure();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
//	float CalculatePressure(ASentinelCharacter* Principal, UBlackboardComponent* BlackboardComponent) const;
};

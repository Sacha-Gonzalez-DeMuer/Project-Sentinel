// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_HasPrincipal.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTDecorator_HasPrincipal : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_HasPrincipal();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};

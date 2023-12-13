// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsObjectInRange.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTDecorator_IsObjectInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsObjectInRange();

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Range")
	float Range;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

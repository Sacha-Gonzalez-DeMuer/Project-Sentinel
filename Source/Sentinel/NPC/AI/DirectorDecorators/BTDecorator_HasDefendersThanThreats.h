// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_HasDefendersThanThreats.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTDecorator_HasDefendersThanThreats : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTDecorator_HasDefendersThanThreats();
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	FBlackboardKeySelector SentinelWithThreats; // sentinel to check for attackers

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool CheckForLess; // if set the comparison will check for less defenders than threats, otherwise more


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

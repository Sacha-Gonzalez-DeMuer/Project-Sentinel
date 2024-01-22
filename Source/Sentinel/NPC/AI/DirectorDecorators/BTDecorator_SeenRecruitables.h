// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_SeenRecruitables.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTDecorator_SeenRecruitables : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTDecorator_SeenRecruitables();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

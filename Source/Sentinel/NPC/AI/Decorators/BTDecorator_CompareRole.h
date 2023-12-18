// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CompareRole.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTDecorator_CompareRole : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category="Settings")
	float Damage;
	
};

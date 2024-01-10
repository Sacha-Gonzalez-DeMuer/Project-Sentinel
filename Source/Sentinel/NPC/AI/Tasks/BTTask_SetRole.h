// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetRole.generated.h"

enum class ERoles : uint8;
/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_SetRole : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetRole();

	UPROPERTY(EditAnywhere)
	ERoles SetToRole;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SentinelTargetKey;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

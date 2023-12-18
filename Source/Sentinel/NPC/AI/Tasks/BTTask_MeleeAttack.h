// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Sentinel/SentinelCharacter.h"
#include "BTTask_MeleeAttack.generated.h"

class ASentinelController;
class ASentinelCharacter;
/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MeleeAttack();


protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
	
	//virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void UpdateTargetLocation(const ASentinelCharacter* Target);

	UPROPERTY()
	ASentinelCharacter* TargetToAttack;

	UPROPERTY()
	ASentinelController* AIController;

	UPROPERTY()
	ASentinelCharacter* ControlledCharacter;
	
	float DashTimer;

	UPROPERTY(EditAnywhere)
	float DashSpeed;

	UPROPERTY(EditAnywhere)
	float Damage;
	
	UPROPERTY(EditAnywhere)
	float DefaultWalkSpeed;

	UPROPERTY(EditAnywhere)
	float DefaultAcceleration;
	
	UPROPERTY(EditAnywhere)
	float OutOfRangeDist;

	void AbortDash(UBehaviorTreeComponent& OwnerComp);
	void ResetMovementSpeed(ASentinelController* SController);

	FVector TargetLocation;
};

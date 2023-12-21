// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ShootTarget.generated.h"

/**
 * 
 */

class ASentinelCharacter;
class ABullet;
class ANPCBase;

UCLASS()
class SENTINEL_API UBTTask_ShootTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_ShootTarget();


private:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	void LookAtTarget() const;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ABullet> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Setup")
	float ShotCooldown = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MuzzlePositionX = 150;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MuzzlePositionY = 70;

	UPROPERTY(EditAnywhere, Category="Setup")
	bool LookAtOnShot = true;

	UPROPERTY()
	ASentinelCharacter* Target;

	UPROPERTY()
	ANPCBase* NPCBase;
};

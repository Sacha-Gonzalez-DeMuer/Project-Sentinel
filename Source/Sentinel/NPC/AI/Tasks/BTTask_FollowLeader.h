// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FollowLeader.generated.h"

class ANPCBase;
class ASentinelCharacter;
class ASentinelController;
/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_FollowLeader : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_FollowLeader();

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LeaderKey;

	UPROPERTY(EditAnywhere, Category= "Following")
	float DistanceBehindLeader = 250.f;

	UPROPERTY(EditAnywhere, Category= "Following")
	float LeaderSightAngleToAvoid = 45.f;
	
	UPROPERTY(EditAnywhere, Category= "Following")
	float ArriveRadius = 50.f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float MinDistanceToThreat = 250.0f;
	
	UPROPERTY(EditAnywhere, Category="Avoidance")
	float ThreatAvoidanceRadius = 250.0f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float ThreatAvoidanceWeight = .5f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float AllyAvoidanceRadius = 250.0f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float AllyAvoidanceWeight = .5f;
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY()
	ANPCBase* NPC;
	
	UPROPERTY()
	ASentinelController* NPCController;

	UPROPERTY()
	ASentinelCharacter* Leader;

	UPROPERTY()
	UPawnMovementComponent* NPCMovement;

	FVector CalculateAvoidance();
	bool IsOnLeaderSight(const FVector& Position);
	FVector Evade(const ASentinelCharacter& ToEvade);
	FVector Arrive(const FVector& Target, float ArrivalRadius);
	FVector Flee(const FVector& TargetPosition);
};

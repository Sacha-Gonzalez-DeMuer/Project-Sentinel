// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteeringBehavior.h"
#include "Sentinel/NPC/AI/Steering/SteeringBehavior.h"
#include "Follow.generated.h"

class ASentinelCharacter;

UCLASS()
class SENTINEL_API UFollow : public USteeringBehavior
{
	GENERATED_BODY()

public:
	UFollow();

	virtual FVector CalculateSteering(const ASentinelCharacter* SteeringAgent) override;

	void SetToFollow(ASentinelCharacter* toFollow);
	
protected:
	UPROPERTY()
	ASentinelCharacter* ToFollow = nullptr;

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


	FVector CalculateAvoidance(const ASentinelCharacter* SteeringAgent);
	bool IsOnLeaderSight(const ASentinelCharacter* SteeringAgent, const FVector& Position);
	FVector Evade(const ASentinelCharacter* SteeringAgent, const ASentinelCharacter& ToEvade);
	FVector Arrive(const ASentinelCharacter* SteeringAgent, const FVector& Target, float ArrivalRadius);
	FVector Flee(const ASentinelCharacter* SteeringAgent, const FVector& TargetPosition);
};
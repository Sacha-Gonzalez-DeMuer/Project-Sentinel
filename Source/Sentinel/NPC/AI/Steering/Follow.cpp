// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Steering/Follow.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UFollow::UFollow()
{
}

FVector UFollow::CalculateSteering(const ASentinelCharacter* SteeringAgent)
{
	if(!ToFollow) return {0,0,0};
	const FVector LeaderLocation = ToFollow->GetActorLocation();
	const FVector LeaderVelocity =  ToFollow->GetMovementComponent()->Velocity.GetSafeNormal();
	
	FVector TargetVelocity;
	
	// Calculate behind point
	const FVector BehindLeader = LeaderLocation - LeaderVelocity * DistanceBehindLeader;
	TargetVelocity += Arrive(SteeringAgent, BehindLeader, ArriveRadius);
	
	// Calculate ahead point
	const FVector AheadLeader = LeaderLocation + LeaderVelocity * DistanceBehindLeader;
	
	if(IsOnLeaderSight(SteeringAgent, AheadLeader))
	{
		TargetVelocity += Evade(SteeringAgent, *ToFollow);
	}
	
	TargetVelocity += CalculateAvoidance(SteeringAgent);

	return TargetVelocity;
}

void UFollow::SetToFollow(ASentinelCharacter* toFollow)
{
	//UE_LOG(LogTemp, Log, TEXT("Following %s"), *ToFollow->GetName());
	ToFollow = toFollow;
}

FVector UFollow::CalculateAvoidance(const ASentinelCharacter* SteeringAgent)
{
	FVector AvoidanceForce;
	const ASentinelController* SentinelController = SteeringAgent->GetSentinelController();
	
	// Threat avoidance
	TSet<ASentinelCharacter*> SeenThreats = SentinelController->GetSeenThreats();
	int32 ThreatCount = 0;
	FVector ThreatAvoidance;

	for (ASentinelCharacter* Threat : SeenThreats)
	{
		FVector FromThreat = SentinelController->GetCharacter()->GetActorLocation() - Threat->GetActorLocation();
		float SqrDistanceToThreat = FromThreat.SquaredLength();

		if (SqrDistanceToThreat < ThreatAvoidanceRadius * ThreatAvoidanceRadius)
		{
			ThreatAvoidance += FromThreat;
			ThreatCount++;
		}
	}

	if (ThreatCount != 0)
	{
		ThreatAvoidance /= ThreatCount;
		ThreatAvoidance.Normalize();
		AvoidanceForce += ThreatAvoidance * ThreatAvoidanceWeight;
	}

	// Ally avoidance
	int32 AllyCount = 0;
	FVector AllyAvoidance;

	ASentinelSquad* SentinelSquad = SteeringAgent->GetSquad();
	TArray<ASentinelCharacter*> Allies = SentinelSquad->GetSentinels();
	Allies.Add(ToFollow);
	
	for (ASentinelCharacter* Ally : Allies)
	{
		FVector FromAlly = SentinelController->GetCharacter()->GetActorLocation() - Ally->GetActorLocation();
		float SqrDistanceToAlly = FromAlly.SquaredLength();

		if (SqrDistanceToAlly < AllyAvoidanceRadius * AllyAvoidanceRadius)
		{
			AllyAvoidance += FromAlly;
			AllyCount++;
		}
	}

	if (AllyCount != 0)
	{
		AllyAvoidance /= AllyCount;
		AllyAvoidance.Normalize();
		AvoidanceForce += AllyAvoidance * AllyAvoidanceWeight;
	}

	return AvoidanceForce;
}

bool UFollow::IsOnLeaderSight(const ASentinelCharacter* SteeringAgent, const FVector& Position)
{
	if (ToFollow)
	{
		constexpr float LeaderSightRadiusSqr = 100.0f * 100.0f;
		return FVector::DistSquared(Position, SteeringAgent->GetActorLocation()) <= LeaderSightRadiusSqr
		|| FVector::DistSquared(ToFollow->GetActorLocation(), SteeringAgent->GetActorLocation()) <= LeaderSightRadiusSqr;
	}

	// Return false if the leader is not valid
	return false;
}

FVector UFollow::Evade(const ASentinelCharacter* SteeringAgent, const ASentinelCharacter& ToEvade)
{
	// Get the direction from the NPC to the target position
	const FVector ToTarget = ToEvade.GetActorLocation() - SteeringAgent->GetActorLocation();

	// Calculate the distance to the target
	const float Distance = ToTarget.Size();

	// Calculate the updates ahead based on the distance and maximum velocity
	int32 UpdatesAhead = FMath::FloorToInt(Distance / SteeringAgent->GetMovementComponent()->GetMaxSpeed());

	// Calculate the future position based on the target's velocity
	FVector FuturePosition = ToEvade.GetActorLocation() + ToEvade.GetVelocity() * UpdatesAhead;

	// Evade the future position using flee behavior
	return Flee(SteeringAgent, FuturePosition);
}

FVector UFollow::Arrive(const ASentinelCharacter* SteeringAgent, const FVector& Target, float ArrivalRadius)
{
	// Calculate the desired velocity towards the target
	FVector ToTarget = Target - SteeringAgent->GetActorLocation();
	float Distance = ToTarget.Size();
	const float halfArrivalRadius = ArriveRadius / 2.0f;

	if(Distance< halfArrivalRadius) return {0,0,0};
	// Check if the NPC is within the arrival radius
	if (Distance < ArrivalRadius)
	{
		// Normalize the vector and scale it based on the arrival radius
		ToTarget.Normalize();
		ToTarget *= ArrivalRadius;

		// Adjust the distance for a smooth deceleration curve
		Distance = FMath::Max(0.0f, Distance - ArrivalRadius);

		// Calculate the speed factor
		float SpeedFactor = Distance / ArrivalRadius;

		// Adjust the speed factor for a smooth deceleration curve
		SpeedFactor = FMath::Pow(SpeedFactor, 1);

		// Apply the adjusted speed factor
		ToTarget *= SpeedFactor;
	}

	// Calculate the steering force towards the desired velocity
	FVector SteeringForce = ToTarget - SteeringAgent->GetVelocity();

	return SteeringForce;
}

FVector UFollow::Flee(const ASentinelCharacter* SteeringAgent,const FVector& TargetPosition)
{
	// Get the direction from the NPC to the target position
	FVector DirectionToTarget = SteeringAgent->GetActorLocation() - TargetPosition;
	DirectionToTarget.Normalize();

	// Calculate the evasion direction (opposite to the direction to the target)
	FVector EvasionDirection = -DirectionToTarget;

	return EvasionDirection;
}


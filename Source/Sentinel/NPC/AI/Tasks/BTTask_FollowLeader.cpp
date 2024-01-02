// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_FollowLeader.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"
#include "Sentinel/NPC/AI/Steering/Follow.h"

UBTTask_FollowLeader::UBTTask_FollowLeader()
{
	NodeName = TEXT("Follow Leader");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FollowLeader::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard =  OwnerComp.GetBlackboardComponent();
	
	NPC = Cast<ANPCBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowLeader task failed"));
		return EBTNodeResult::Failed;
	}
	NPCController = NPC->GetSentinelController();
	
	Leader = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(LeaderKey.SelectedKeyName));
	if (!Leader)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowLeader task failed"));
		return EBTNodeResult::Failed;
	}

	UFollow* FollowComponent = NPCController->GetFollowComponent();
	if(!FollowComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowLeader task failed"));

		FinishLatentTask(OwnerComp,  EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	FollowComponent->SetToFollow(Leader);
	FinishLatentTask(OwnerComp,  EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

// void UBTTask_FollowLeader::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
// {
// 	const FVector NPCLocation = NPC->GetActorLocation();
// 	const FVector LeaderLocation = Leader->GetActorLocation();
// 	const FVector LeaderVelocity =  Leader->GetMovementComponent()->Velocity.GetSafeNormal();
// 	
// 	FVector TargetVelocity;
// 	
// 	// Calculate behind point
// 	const FVector BehindLeader = LeaderLocation - LeaderVelocity * DistanceBehindLeader;
// 	TargetVelocity += Arrive(BehindLeader, ArriveRadius);
// 	
// 	// Calculate ahead point
// 	const FVector AheadLeader = LeaderLocation + LeaderVelocity * DistanceBehindLeader;
// 	
// 	if(IsOnLeaderSight(AheadLeader))
// 	{
// 		TargetVelocity += Evade(*Leader);
// 	}
// 	
// 	TargetVelocity += CalculateAvoidance();
// 	
// 	// Use NavMesh to move the NPC to the new location
// 	FVector NewLocation = NPCLocation + TargetVelocity;
// 	DrawDebugSphere(GetWorld(), NewLocation, 150.0f, 8, FColor::Green, false, -1, 0, 1);
//
// 	FAIMoveRequest MoveReq;
// 	MoveReq.SetGoalLocation(NewLocation);
// 	MoveReq.SetAcceptanceRadius(10.0f); // Set your desired acceptance radius
// 	NPCController->MoveTo(MoveReq);
// }
//
// FVector UBTTask_FollowLeader::CalculateAvoidance()
// {
// 	FVector AvoidanceForce;
//
// 	// Threat avoidance
// 	TSet<ASentinelCharacter*> SeenThreats = NPCController->GetSeenThreats();
// 	int32 ThreatCount = 0;
// 	FVector ThreatAvoidance;
//
// 	for (ASentinelCharacter* Threat : SeenThreats)
// 	{
// 		FVector FromThreat = NPCController->GetCharacter()->GetActorLocation() - Threat->GetActorLocation();
// 		float SqrDistanceToThreat = FromThreat.SquaredLength();
//
// 		if (SqrDistanceToThreat < ThreatAvoidanceRadius * ThreatAvoidanceRadius)
// 		{
// 			ThreatAvoidance += FromThreat;
// 			ThreatCount++;
// 		}
// 	}
//
// 	if (ThreatCount != 0)
// 	{
// 		ThreatAvoidance /= ThreatCount;
// 		ThreatAvoidance.Normalize();
// 		AvoidanceForce += ThreatAvoidance * ThreatAvoidanceWeight;
// 	}
//
// 	// Ally avoidance
// 	int32 AllyCount = 0;
// 	FVector AllyAvoidance;
//
// 	ASentinelSquad* SentinelSquad = NPC->GetSquad();
// 	
// 	TArray<ASentinelCharacter*> Allies = SentinelSquad->GetSentinels();
// 	Allies.Add(Leader);
// 	
// 	for (ASentinelCharacter* Ally : Allies)
// 	{
// 		FVector FromAlly = NPCController->GetCharacter()->GetActorLocation() - Ally->GetActorLocation();
// 		float SqrDistanceToAlly = FromAlly.SquaredLength();
//
// 		if (SqrDistanceToAlly < AllyAvoidanceRadius * AllyAvoidanceRadius)
// 		{
// 			AllyAvoidance += FromAlly;
// 			AllyCount++;
// 		}
// 	}
//
// 	if (AllyCount != 0)
// 	{
// 		AllyAvoidance /= AllyCount;
// 		AllyAvoidance.Normalize();
// 		AvoidanceForce += AllyAvoidance * AllyAvoidanceWeight;
// 	}
//
// 	return AvoidanceForce;
// }
//
// bool UBTTask_FollowLeader::IsOnLeaderSight(const FVector& Position)
// {
// 	if (Leader)
// 	{
// 		constexpr float LeaderSightRadiusSqr = 100.0f * 100.0f;
// 		return FVector::DistSquared(Position, NPC->GetActorLocation()) <= LeaderSightRadiusSqr
// 		|| FVector::DistSquared(Leader->GetActorLocation(), NPC->GetActorLocation()) <= LeaderSightRadiusSqr;
// 	}
//
// 	// Return false if the leader is not valid
// 	return false;
// }
//
// FVector UBTTask_FollowLeader::Evade(const ASentinelCharacter& ToEvade)
// {
// 	// Get the direction from the NPC to the target position
// 	const FVector ToTarget = ToEvade.GetActorLocation() - NPC->GetActorLocation();
//
// 	// Calculate the distance to the target
// 	const float Distance = ToTarget.Size();
//
// 	// Calculate the updates ahead based on the distance and maximum velocity
// 	int32 UpdatesAhead = FMath::FloorToInt(Distance / NPC->GetMovementComponent()->GetMaxSpeed());
//
// 	// Calculate the future position based on the target's velocity
// 	FVector FuturePosition = ToEvade.GetActorLocation() + ToEvade.GetVelocity() * UpdatesAhead;
//
// 	// Evade the future position using flee behavior
// 	return Flee(FuturePosition);
// }
//
// FVector UBTTask_FollowLeader::Flee(const FVector& TargetPosition)
// {
// 	// Get the direction from the NPC to the target position
// 	FVector DirectionToTarget = NPC->GetActorLocation() - TargetPosition;
// 	DirectionToTarget.Normalize();
//
// 	// Calculate the evasion direction (opposite to the direction to the target)
// 	FVector EvasionDirection = -DirectionToTarget;
//
// 	return EvasionDirection;
// }
//
// FVector UBTTask_FollowLeader::Arrive(const FVector& Target, float ArrivalRadius)
// {
// 	// Calculate the desired velocity towards the target
// 	FVector ToTarget = Target - NPC->GetActorLocation();
// 	float Distance = ToTarget.Size();
//
// 	// Check if the NPC is within the arrival radius
// 	if (Distance < ArrivalRadius)
// 	{
// 		// Normalize the vector and scale it based on the arrival radius
// 		ToTarget.Normalize();
// 		ToTarget *= ArrivalRadius;
//
// 		// Adjust the distance for a smooth deceleration curve
// 		Distance = FMath::Max(0.0f, Distance - ArrivalRadius);
//
// 		// Calculate the speed factor
// 		float SpeedFactor = Distance / ArrivalRadius;
//
// 		// Adjust the speed factor for a smooth deceleration curve
// 		SpeedFactor = FMath::Pow(SpeedFactor, 1);
//
// 		// Apply the adjusted speed factor
// 		ToTarget *= SpeedFactor;
// 	}
//
// 	// Calculate the steering force towards the desired velocity
// 	FVector SteeringForce = ToTarget - NPC->GetVelocity();
//
// 	return SteeringForce;
// }
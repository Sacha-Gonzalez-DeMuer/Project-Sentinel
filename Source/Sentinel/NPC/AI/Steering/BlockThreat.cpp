// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Steering/BlockThreat.h"

#include "Follow.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"

// Sets default values for this component's properties
UBlockThreat::UBlockThreat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBlockThreat::SetBlackboard(UBlackboardComponent* blackboardComponent)
{
	//BlackboardComponent = blackboardComponent;
}


// Called when the game starts
void UBlockThreat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FVector UBlockThreat::CalculateSteering(const ASentinelCharacter* SteeringAgent)
{
	UBlackboardComponent* Blackboard = SteeringAgent->GetSentinelController()->BlackboardComponent;
	
	if(!Blackboard)
		UE_LOG(LogTemp, Log, TEXT("No blackboard lol?"));

	ASentinelCharacter* Principal = Cast<ASentinelCharacter>(
		Blackboard->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));

	if(!Principal)
	{
		return {0,0,0};
	}

	FVector Steering;
	
	const FVector NPCLocation = SteeringAgent->GetActorLocation();
	const FVector PrincipalLocation = Principal->GetActorLocation();
    const FVector ThreatLocation = SteeringAgent->GetSentinelController()->GetThreatLocation();

	if(ThreatLocation == NPCLocation) // no threat found
	{
		UE_LOG(LogTemp, Warning, TEXT("NO THREATS, FOLLOWING LEADER"));
		
		return SteeringAgent->GetSentinelController()->GetFollowComponent()->CalculateSteering(SteeringAgent);
	}
	
	const FVector ToThreat = ThreatLocation - PrincipalLocation;
    const FVector BlockingPosition = PrincipalLocation + ToThreat.GetSafeNormal() *
	    DistanceInFrontOfPrincipal;

	

	Steering = Arrive(SteeringAgent, BlockingPosition, ArriveRadius);
	Steering += CalculateAvoidance(SteeringAgent);
	DrawDebugSphere(GetWorld(), BlockingPosition, 50.0f, 12, FColor::Red, false, -1, 0, 2.0f);
	DrawDebugLine(GetWorld(), NPCLocation, NPCLocation + Steering, FColor::Cyan, false, -1, 0, 2.0f);
	
	return Steering;


	
	
	
	
	const FVector ToPrincipal = (PrincipalLocation - NPCLocation);
	const float SqrDistanceToPrincipal = ToPrincipal.SquaredLength();
	
}

FVector UBlockThreat::CalculateAvoidance(const ASentinelCharacter* SteeringAgent)
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

FVector UBlockThreat::Arrive(const ASentinelCharacter* SteeringAgent, const FVector& Target, float ArrivalRadius) const
{
	FVector DesiredVelocity = Target - SteeringAgent->GetActorLocation();
	const float Distance = DesiredVelocity.Size();
	const float MaxSpeed = SteeringAgent->GetMovementComponent()->GetMaxSpeed();
	
	if (Distance < ArrivalRadius)
	{
		// Inside the slowing area, apply slowing effect
		const float SlowingFactor = MaxSpeed * (Distance / ArrivalRadius);
		DesiredVelocity = DesiredVelocity.GetSafeNormal() * SlowingFactor;
	}
	else
	{
		// Outside the slowing area, use max speed
		DesiredVelocity = DesiredVelocity.GetSafeNormal() * MaxSpeed;
	}

	// Calculate steering force
	return DesiredVelocity - SteeringAgent->GetMovementComponent()->Velocity;

}

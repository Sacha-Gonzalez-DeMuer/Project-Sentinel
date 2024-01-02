// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_BlockThreat.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UBTTask_BlockThreat::UBTTask_BlockThreat()
	: DistanceInFrontOfPrincipal(600.0f)
	, AvoidanceWeight(.5f)
	, AcceptanceRadius(5.0f)
	, NPC(nullptr)
	, MaxAvoidanceDistance(500.0f)
	, NPCController(nullptr)
	, Principal(nullptr)
	, MinDistanceToThreat(300.0f)
	, ThreatAvoidanceWeight(.5f)
	, ThreatAvoidanceRadius(200.f)
{
	NodeName = TEXT("Block Threat");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BlockThreat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard =  OwnerComp.GetBlackboardComponent();
	Principal = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));

	if(!Principal)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockThreat task failed"));
		return EBTNodeResult::Failed;
	}

	
	NPC = Cast<ANPCBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockThreat task failed"));
		return EBTNodeResult::Failed;
	}
	NPCMovement = NPC->GetMovementComponent();
	NPCController = NPC->GetSentinelController();
	
	Principal = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));
	if (!Principal)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockThreat task failed"));
		return EBTNodeResult::Failed;
	}

	

	return EBTNodeResult::InProgress;
}

void UBTTask_BlockThreat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const FVector NPCLocation = NPC->GetActorLocation();
	const FVector PrincipalLocation = Principal->GetActorLocation();
    const FVector ThreatLocation = NPC->GetSentinelController()->GetThreatLocation();
	const FVector ToThreat = ThreatLocation - PrincipalLocation;
    const FVector DesiredLocation = PrincipalLocation + ToThreat.GetSafeNormal() *
	    DistanceInFrontOfPrincipal;
	
	if(FVector::DistSquared(DesiredLocation, NPCLocation) < AcceptanceRadius * AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
    const FVector DesiredDirection = (DesiredLocation - NPCLocation).GetSafeNormal();
	const FVector ToPrincipal = (PrincipalLocation - NPCLocation);
	const float SqrDistanceToPrincipal = ToPrincipal.SquaredLength();

	FVector SteeringDirection = DesiredDirection.GetSafeNormal() * BlockLocationWeight;
	SteeringDirection += Principal->GetMovementComponent()->Velocity.GetSafeNormal(); // follow along with principal

    // Draw a debug sphere at the desired position
    DrawDebugSphere(GetWorld(), DesiredLocation, 50.0f, 8, FColor::Green, false, -1, 0, 1);
//	SteeringDirection += CalculateThreatAvoidance();
//	SteeringDirection += CalculateAllyAvoidance();
	DrawDebugSphere(GetWorld(), ThreatLocation, 50.0f, 8, FColor::Red, false, -1, 0, 1);

	// Avoid the principal
	if(SqrDistanceToPrincipal < MinDistanceToPrincipal * MinDistanceToPrincipal || FVector::DotProduct(ToPrincipal, ToThreat) > 0)
	{ 
		FVector AvoidanceDirection = FVector::CrossProduct(ToPrincipal, FVector::UpVector).GetSafeNormal();
		SteeringDirection += AvoidanceDirection * 1.0f + DistanceInFrontOfPrincipal / (SqrDistanceToPrincipal+1.0f);
	}
	
	// Move
    if (NPCMovement)
    {
    	const FVector SteeringVelocity = SteeringDirection * NPCMovement->GetMaxSpeed();
        const FVector Acceleration = (SteeringVelocity - NPCMovement->Velocity) / DeltaSeconds;
        NPCMovement->AddInputVector(Acceleration);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[UBTTask_BlockThreat::TickTask] NPCController is nullptr!"));
    }
}

FVector UBTTask_BlockThreat::CalculateThreatAvoidance()
{
	FVector ThreatAvoidance;
	
	TSet<ASentinelCharacter*> SeenThreats = NPCController->GetSeenThreats();
	for(ASentinelCharacter* Threat : SeenThreats)
	{
		FVector FromThreat =  NPCController->GetCharacter()->GetActorLocation() - Threat->GetActorLocation();
		float SqrDistanceToThreat =  FromThreat.SquaredLength();
		
		// This target is a threat, apply avoidance logic
		if(FromThreat.SquaredLength() < ThreatAvoidanceRadius * ThreatAvoidanceRadius)
		{
			FVector AvoidanceDirection = FromThreat.GetSafeNormal();
			ThreatAvoidance  += AvoidanceDirection * 1.0f + ThreatAvoidanceRadius / (SqrDistanceToThreat+1.0f);
		}
	}

	ThreatAvoidance.Normalize();
	return ThreatAvoidance * ThreatAvoidanceWeight;
}
// FVector UBTTask_BlockThreat::CalculateAllyAvoidance()
// {
// 	FVector AllyAvoidance;
//
// 	ASentinelSquad* SentinelSquad = NPC->GetSquad();
//
// 	for(ASentinelCharacter* Ally : SentinelSquad->GetSentinels())
// 	{
// 		FVector FromThreat =  NPCController->GetCharacter()->GetActorLocation() - Ally->GetActorLocation();
// 		float SqrDistanceToThreat =  FromThreat.SquaredLength();
// 		
// 		// This target is a threat, apply avoidance logic
// 		if(FromThreat.SquaredLength() < AllyAvoidanceRadius * AllyAvoidanceRadius)
// 		{
// 			FVector AvoidanceDirection = FromThreat.GetSafeNormal();
// 			AllyAvoidance  += AvoidanceDirection * 1.0f + ThreatAvoidanceRadius / (SqrDistanceToThreat+1.0f);
// 		}
// 	}
//
// 	return AllyAvoidance * AllyAvoidanceWeight;
// }
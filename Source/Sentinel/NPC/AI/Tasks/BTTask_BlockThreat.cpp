// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_BlockThreat.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UBTTask_BlockThreat::UBTTask_BlockThreat()
	: DistanceInFrontOfPrincipal(600.0f)
	, AvoidanceWeight(.5f)
	, NPC(nullptr)
	, NPCController(nullptr)
	, MaxAvoidanceDistance(500.0f)
	, Principal(nullptr)
	, MinDistanceToThreat(300.0f)

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
	
	// Get the owning actor
	AActor* OwnerActor = OwnerComp.GetOwner();

	// Check if the owner actor is valid
	if (OwnerActor)
	{
		// Print the name of the owner actor
		UE_LOG(LogTemp, Warning, TEXT("Owner Actor Name: %s"), *OwnerActor->GetName());
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		// Print a warning if the owner actor is not valid
		UE_LOG(LogTemp, Warning, TEXT("Owner Actor is invalid"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	const FVector ThreatLocation = NPC->GetSentinelController()->GetThreatLocation();
	
	// Calculate the desired position between the principal and the threat
	const FVector PrincipalLocation = Principal->GetActorLocation();
	const FVector PrincipalToThreat = ThreatLocation - PrincipalLocation;
	const FVector DesiredDirection = PrincipalToThreat.GetSafeNormal() * DistanceInFrontOfPrincipal;
	const FVector ProtectivePosition = PrincipalLocation + DesiredDirection;
	const FVector ProtectiveToThreat = ThreatLocation - ProtectivePosition;
	const float ProtectivePositionDistanceToThreat = PrincipalToThreat.Length() - DistanceInFrontOfPrincipal;
	const float DistanceToThreat = PrincipalToThreat.Length();

	// Additional influence factor based on the threat's proximity
	const float ThreatInfluenceFactor = FMath::Clamp(1.0f - ProtectivePositionDistanceToThreat / MaxAvoidanceDistance, 0.0f, 1.0f);
	const FVector AvoidanceDirection = -ProtectiveToThreat.GetSafeNormal() * ThreatInfluenceFactor;

	// Combine the desired direction and avoidance direction with weights
	const FVector CombinedDirection = DesiredDirection * (1.0f - AvoidanceWeight) - AvoidanceDirection * AvoidanceWeight;
	const FVector DesiredPosition = PrincipalLocation + CombinedDirection;
	//PrincipalLocation + (PrincipalLocation - ThreatLocation).GetSafeNormal() * DistanceInFrontOfPrincipal;

	// Draw a debug sphere at the desired position
	//DrawDebugSphere(GetWorld(), DesiredPosition, 50.0f, 8, FColor::Green, false, -1, 0, 1);

	// Move the NPC towards the desired position
	if (NPCController)
	{
		NPCController->MoveTo(DesiredPosition);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCController is nullptr!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_MeleeAttack.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sentinel/NPC/AI/SentinelController.h"

class UNavigationSystemV1;

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TargetToAttack = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (TargetToAttack == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AIController = Cast<ASentinelController>(OwnerComp.GetAIOwner());

	ControlledCharacter = Cast<ASentinelCharacter>(AIController->GetPawn());
	if(ControlledCharacter == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
		
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

void UBTTask_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(AIController == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("FORCED DASH ABORT"))

		AbortDash(OwnerComp);
		
		return;
	}

	const float DistanceToTarget = FVector::Distance(TargetLocation, ControlledCharacter->GetActorLocation());
	const FVector EnemyForwardVector = ControlledCharacter->GetActorForwardVector();
	FVector DirectionToPlayer = (TargetToAttack->GetActorLocation() - ControlledCharacter->GetActorLocation()).GetSafeNormal();

	// Check if the player is behind the enemy
	float DotProduct = FVector::DotProduct(DirectionToPlayer, EnemyForwardVector);
	if (DotProduct < 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Player is behind enemy. ABORT DASH"))

		AbortDash(OwnerComp);

		return;
	}

	// Move the character towards the target location on the navmesh with the specified speed
	auto characterMovement = ControlledCharacter->GetCharacterMovement();
	if(characterMovement != nullptr)
	{
		characterMovement->MaxWalkSpeed = DashSpeed;
		characterMovement->MaxAcceleration = DashSpeed * 10;
	}
	
	if(DistanceToTarget >= OutOfRangeDist)
	{
		AbortDash(OwnerComp);
	}
	
	// Check if the character has reached the target or an acceptable distance
	constexpr float AcceptableDistance = 150.0f; // Adjust the acceptable distance as needed
	
	//const float TimeSinceLastMoveUpdate = GetWorld()->GetTimeSeconds() - MoveToUpdateTime;
	//if(TimeSinceLastMoveUpdate > MoveToUpdateCooldownTime)
	//{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TargetLocation);
	///	MoveToUpdateTime = GetWorld()->GetTimeSeconds();
	//}

	
	if (DistanceToTarget <= AcceptableDistance)
	{
		ControlledCharacter->SetActorLocation(TargetLocation);
		ControlledCharacter->Attack(TargetToAttack, Damage);

		// Reset the movement speed to default
		ResetMovementSpeed(AIController);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_MeleeAttack::UpdateTargetLocation(const ASentinelCharacter* Target)
{
	FVector TargetDashLocation = Target->GetActorLocation();
	FVector DirectionToTarget = TargetLocation - AIController->GetPawn()->GetActorLocation();
	DirectionToTarget.Z = 0; // Ensure no vertical movement
	DirectionToTarget.Normalize();

	// Adjust the target position to be slightly further from the player
	const FVector DesiredLocation = TargetLocation + DirectionToTarget;

	// Use the navigation system to find the nearest valid location on the NavMesh
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		FNavLocation NavLocation;
		if (NavSys->GetRandomReachablePointInRadius(DesiredLocation, 100.0f, NavLocation))
		{
			TargetLocation = NavLocation.Location;
		}
		else
		{
			TargetLocation = DesiredLocation - DirectionToTarget * 500;
		}
	}

	TargetLocation = TargetDashLocation;
}

void UBTTask_MeleeAttack::AbortDash(UBehaviorTreeComponent& OwnerComp)
{
	AIController->StopMovement();
	ResetMovementSpeed(AIController);
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTTask_MeleeAttack::ResetMovementSpeed(ASentinelController* SController)
{
	if (SController->GetCharacter() != nullptr)
	{
		SController->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed; // Default walk speed
		SController->GetCharacter()->GetCharacterMovement()->MaxAcceleration = DefaultAcceleration; // Default Acceleration speed
	}
}

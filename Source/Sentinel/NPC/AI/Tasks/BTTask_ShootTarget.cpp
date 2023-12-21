// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_ShootTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/Actors/Bullet.h"
#include "Sentinel/NPC/NPCBase.h"

UBTTask_ShootTarget::UBTTask_ShootTarget()
	: Target(nullptr)
	, NPCBase(nullptr)
{
	NodeName = TEXT("Shoot Target");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ShootTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Target = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	NPCBase = Cast<ANPCBase>(OwnerComp.GetAIOwner()->GetPawn());
	
	return EBTNodeResult::InProgress;
}

void UBTTask_ShootTarget::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	
	if (!Target || !NPCBase)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Failed);
		return;
	}

	// Check for allies in the path before shooting
	FVector StartLocation = NPCBase->GetActorLocation();
	FVector EndLocation = Target->GetActorLocation();
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(NPCBase); // Ignore the shooting NPC


	// Perform raycastss
	if (ownerComp.GetOwner()->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, CollisionParams))
	{
		// Check if the hit actor is an ally
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UBTTask_ShootTarget::TickTask] Hit actor found: %s"), *HitActor->GetName());

			// Check if the hit actor is an ally
			if (ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(HitActor))
			{
				if (Sentinel->IsAlly(NPCBase->GetFactionIdx()))
				{
					// Ally in the path, stop shooting
					UE_LOG(LogTemp, Warning, TEXT("[UBTTask_ShootTarget::TickTask] Ally in the path, stop shooting."));
					FinishLatentTask(ownerComp, EBTNodeResult::Failed);
					return;
				}
			}
		}
	}
	
	// Set the spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = ownerComp.GetOwner();
	SpawnParams.Instigator = NPCBase;
	
	// Set the spawn location and rotation
	const FVector SpawnLocation = NPCBase->GetActorLocation();
	constexpr float BulletSpeed = 4000.0f;
	
	// Calculate the predicted position of the player based on their current velocity and the expected time of flight for the bullet
	const float TimeOfFlight = (Target->GetActorLocation() - SpawnLocation).Size() / BulletSpeed; // assuming BulletSpeed is 4000
	const FVector PredictedTargetPosition = Target->GetActorLocation() + Target->GetVelocity() * TimeOfFlight;
	
	// Adjust the rotation to aim towards the predicted target position
	const FRotator SpawnRotation = (PredictedTargetPosition - SpawnLocation).Rotation();
	
	// Spawn the projectile
	ABullet* SpawnedProjectile = SpawnParams.Owner->GetWorld()->SpawnActor<ABullet>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	//if(ANPCBase* NPCSentinel = Cast<ASentinelCharacter>(NPCBase))
	
	if (!SpawnedProjectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShootTarget.cpp: Failed to spawn projectile"))
		FinishLatentTask(ownerComp, EBTNodeResult::Failed);
		return;
	}
	SpawnedProjectile->Initialize(NPCBase);
	
	if(LookAtOnShot) LookAtTarget();
	
	// Check if the projectile was successfully spawned
	if (SpawnedProjectile)
	{
		// Perform any additional setup for the spawned projectile here
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Failed);
	}
}


FString UBTTask_ShootTarget::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}

void UBTTask_ShootTarget::LookAtTarget() const
{
	NPCBase->SetActorRotation((Target->GetActorLocation() - NPCBase->GetActorLocation()).Rotation());
}

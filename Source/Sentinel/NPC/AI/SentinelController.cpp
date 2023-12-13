// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/SentinelController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sentinel/NPC/NPCBase.h"
#include "BlackboardKeys.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/SentinelPlayerCharacter.h"
#include "Sentinel/Components/HealthComponent.h"

ASentinelController::ASentinelController(FObjectInitializer const& objectInit)
	: RetargetingInterval(1.0f)
	  , ThreatUpdateInterval(.5f)
	  , RetargetingIntervalTimer(0.1f)
	  , ThreatUpdateTimer(0.1f)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ASentinelController::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* ControlledPawn = GetPawn())
		NPCBase = Cast<ANPCBase>(ControlledPawn);

	if (IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
	}

	InitializeBlackboardKeys();
}

void ASentinelController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Retargeting timer
	if (RetargetingIntervalTimer > 0)
	{
		RetargetingIntervalTimer -= DeltaSeconds;
		if (RetargetingIntervalTimer <= 0)
		{
			RecalculateTargetPriority();
			RetargetingIntervalTimer = RetargetingInterval;
		}
	}

	// Threat calculation timer
	if (ThreatUpdateTimer > 0)
	{
		ThreatUpdateTimer -= DeltaSeconds;
		if (ThreatUpdateTimer <= 0)
		{
			RecalculateThreatToTarget();
			ThreatUpdateTimer = ThreatUpdateInterval;
		}
	}
}

void ASentinelController::UpdatePrincipal(ASentinelCharacter* NewPrincipal) const
{
	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentPrincipal), NewPrincipal);
}

void ASentinelController::UpdateTarget(ASentinelCharacter* NewTarget) const
{
	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), NewTarget);
}

void ASentinelController::OnSeePawn(APawn* SeenPawn)
{
	if (ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(SeenPawn))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(SeenPawn->GetController()))
		{
			// Seen player!
			// UE_LOG(LogTemp, Log, TEXT("Seen player!"));
		}
		else if (Cast<ANPCBase>(Sentinel))
		{
			int SSFactionIdx = Sentinel->GetFaction();
			// int SSSquadIdx = Sentinel->GetSquad();

			if (SSFactionIdx != NPCBase->GetFaction())
			{
				AddThreat(Sentinel);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Seen unknown pawn"));
		}
	}
}


float ASentinelController::GetThreatToTarget() const
{
	return ThreatToTarget;
}

float ASentinelController::GetThreatToPrincipal() const
{
	return ThreatToPrincipal;
}

bool ASentinelController::HasTarget() const
{
	return BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentTarget)) != nullptr;
}

void ASentinelController::AddThreat(ASentinelCharacter* NewThreat)
{
	SeenThreats.Add(NewThreat);
}

void ASentinelController::SetAttacking(bool Attacking)
{
	bIsAttacking = Attacking;
}

bool ASentinelController::IsAttacking() const
{
	return bIsAttacking;
}

void ASentinelController::InitializeBlackboardKeys()
{
	if (BlackboardComponent)
	{
		if (NPCBase)
			BlackboardComponent->SetValueAsObject(FName(BBKeys::SelfActor), NPCBase);
		else
			UE_LOG(LogTemp, Error, TEXT("NPCBase not linked to SentinelControllers Blackboard."));


		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (NPCBase->IsPlayerTarget())
			{
				BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), PlayerPawn);
			}

			if (NPCBase->IsPlayerPrincipal())
			{
				BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentPrincipal), PlayerPawn);
			}

			if (ASentinelPlayerCharacter* PlayerCharacter = Cast<ASentinelPlayerCharacter>(PlayerPawn))
			{
				Player = PlayerCharacter;
				BlackboardComponent->SetValueAsObject(FName(BBKeys::Player), PlayerCharacter);
			}
		}
	}
}

void ASentinelController::SetDefaultTarget() const
{
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (NPCBase->IsPlayerTarget())
		{
			BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), PlayerPawn);
		}
		else
		{
			BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), nullptr);
		}
	}
}

float ASentinelController::EvaluateThreatToPrincipal(const ASentinelCharacter* Threat) const
{
	if (const ASentinelCharacter* Principal = GetPrincipal())
	{
		// Calculate distance between agent and threat
		const float Distance = FVector::Dist(Threat->GetActorLocation(), Principal->GetActorLocation());

		// Calculate health ratio (normalized between 0 and 1)
		const float HealthRatio = Threat->GetHealthComponent()->GetHealth() / Threat->GetHealthComponent()->
			GetMaxHealth();

		// Might also consider attack power or other relevant factors

		// Weight for each factor (adjust based on importance)
		constexpr float DistanceWeight = 0.5f;
		constexpr float HealthWeight = 0.5f;

		// Calculate overall threat score
		const float ThreatScore = (Distance * DistanceWeight) + (HealthRatio * HealthWeight);

		return ThreatScore;
	}

	return 0;
}

float ASentinelController::EvaluateThreatToTarget() const
{
	if (const ASentinelCharacter* Principal = GetPrincipal())
	{
		// Get the AI controller's path-following component
		const UPathFollowingComponent* PathComponent = GetPathFollowingComponent();

		if (PathComponent)
		{
			// Get the remaining path length from the movement component
			float RemainingPathLength = PathComponent->GetRemainingPathCost();

			// Normalize the distance based on some reference value (e.g., maximum possible distance)
			const float MaxPossibleDistance = 1000.0f; // Adjust as needed
			float NormalizedDistance = FMath::Clamp(RemainingPathLength / MaxPossibleDistance, 0.0f, 1.0f);

			// Calculate health ratio (normalized between 0 and 1)
			const float HealthRatio = NPCBase->GetHealthComponent()->GetHealth() / NPCBase->GetHealthComponent()->
				GetMaxHealth();

			// Weight for each factor (adjust based on importance)
			constexpr float DistanceWeight = 0.5f;
			constexpr float HealthWeight = 0.5f;

			// Calculate overall threat score
			const float ThreatScore = (NormalizedDistance * DistanceWeight) + (HealthRatio * HealthWeight);

			return ThreatScore;
		}
	}

	return 0;
}

void ASentinelController::SetTarget(APawn* NewTarget) const
{
	UE_LOG(LogTemp, Log, TEXT("New target set."));
	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), NewTarget);
}


void ASentinelController::RecalculateTargetPriority()
{
	// If there are seen threats, evaluate and set the highest priority threat as the target
	if (!SeenThreats.IsEmpty())
	{
		float HighestPriority = TNumericLimits<float>::Min();
		ASentinelCharacter* HighestPriorityThreat = nullptr;

		for (ASentinelCharacter* Threat : SeenThreats)
		{
			if(!IsValid(Threat))
			{
				SeenThreats.Remove(Threat);
				continue;
			}
			
			// Evaluate threat priority using the provided function
			const float ThreatPriority = Threat->GetSentinelController()->GetThreatToTarget();

			// Update the highest priority threat
			if (ThreatPriority > HighestPriority)
			{
				HighestPriority = ThreatPriority;
				HighestPriorityThreat = Threat;
			}
		}

		// Set the highest priority threat as the target
		SetTarget(HighestPriorityThreat);
	}
	else // No threats are seen
	{
		SetDefaultTarget();
	}
}

void ASentinelController::RecalculateThreatToTarget()
{
	if (const UPathFollowingComponent* Pathing = GetPathFollowingComponent())
	{
		float BaseThreat = 2000.0f;  // Adjust this as needed
		ThreatToTarget = BaseThreat / (Pathing->GetRemainingPathCost() + 1.0f);  // Adding 1 to avoid division by zero
		UE_LOG(LogTemp, Log, TEXT("Threat to target %f"), ThreatToTarget);
	}
}

void ASentinelController::RecalculateThreatToPrincipal()
{
	ThreatToPrincipal = 0;
}

ASentinelCharacter* ASentinelController::GetPrincipal() const
{
	return Cast<ASentinelCharacter>(
		BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));
}

ASentinelCharacter* ASentinelController::GetTarget() const
{
	return Cast<ASentinelCharacter>(
		BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentTarget)));
}

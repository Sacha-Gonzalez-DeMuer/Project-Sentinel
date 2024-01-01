// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/SentinelController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sentinel/NPC/NPCBase.h"
#include "BlackboardKeys.h"
#include "NavigationSystem.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/SentinelPlayerCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/Actors/SentinelFaction.h"
#include "Sentinel/NPC/SentinelDirector.h"


ASentinelController::ASentinelController(const FObjectInitializer& ObjectInitializer)
	: RetargetingInterval(1.0f),
	ThreatUpdateInterval(.5f),
	RetargetingIntervalTimer(0.1f),
	ThreatUpdateTimer(0.1f)
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

}

void ASentinelController::SetPrincipal(ASentinelCharacter* NewPrincipal) const
{
	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentPrincipal), NewPrincipal);
}

void ASentinelController::SetTarget(ASentinelCharacter* NewTarget) const
{
	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), NewTarget);
}

void ASentinelController::SetDefaultTarget()
{
	if (NPCBase->IsPlayerTarget())
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), PlayerPawn);
		}
	else BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), nullptr);

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
			int SSFactionIdx = Sentinel->GetFactionIdx();
			// int SSSquadIdx = Sentinel->GetSquad();

			if (SSFactionIdx != NPCBase->GetFactionIdx())
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


float ASentinelController::GetThreatToTarget() 
{
	RecalculateThreatToTarget();
	return ThreatToTarget;
}

float ASentinelController::GetThreatToPrincipal() const
{
	return ThreatToPrincipal;
}

FVector ASentinelController::GetThreatLocation() const
{
	if(const ASentinelCharacter* Threat = GetCurrentThreat())
		return Threat->GetActorLocation();

	// no threat found
	return NPCBase->GetActorLocation();
}

void ASentinelController::AddSeenThreat(ASentinelCharacter* NewThreat)
{
	SeenThreats.Add(NewThreat);
}

void ASentinelController::DisableBehaviorTree()
{
	if (BehaviorTreeComponent)
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void ASentinelController::EnableBehaviorTree()
{
	if (BehaviorTreeComponent && BehaviorTree) 
	{
		BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::Looped);
	}
}

void ASentinelController::OnDeath()
{
	BlackboardComponent->SetValueAsBool(FName(BBKeys::IsAlive), false);
	DisableBehaviorTree();
}

void ASentinelController::OnLastStand()
{
	BlackboardComponent->SetValueAsBool(FName(BBKeys::IsAlive), true);
	EnableBehaviorTree();
}

bool ASentinelController::HasTarget() const
{
	return BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentTarget)) != nullptr;
}

ASentinelCharacter* ASentinelController::GetCurrentThreat() const
{
	if(ASentinelCharacter* CurrentThreat = Cast<ASentinelCharacter>(BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentTarget))))
		return CurrentThreat;

	return nullptr;
}

void ASentinelController::AddThreat(ASentinelCharacter* NewThreat)
{
	SeenThreats.Add(NewThreat);
}

void ASentinelController::SetAttacking(bool Attacking)
{
	bIsAttacking = Attacking;
}

void ASentinelController::UpdateRetargetingTimer(float DeltaSeconds)
{
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
}

void ASentinelController::UpdateThreatToTargetTimer(float DeltaSeconds)
{
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


		BlackboardComponent->SetValueAsBool(FName(BBKeys::IsAlive), true);
		
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

			ASentinelSquad* Squad = NPCBase->GetSquad();
			BlackboardComponent->SetValueAsObject(FName(BBKeys::Squad), Squad);

			ASentinelFaction* Faction = NPCBase->GetFaction();
			BlackboardComponent->SetValueAsObject(FName(BBKeys::Faction), Faction);

			SetRole(ERoles::Escort);
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


float ASentinelController::EvaluateThreatToPrincipal(ASentinelCharacter* Threat)
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

float ASentinelController::EvaluateThreatToTarget()
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


void ASentinelController::RecalculateTargetPriority()
{
	// If there are seen threats, evaluate and set the highest priority threat as the target
	if (!SeenThreats.IsEmpty())
	{
		float HighestPriority = TNumericLimits<float>::Min();
		ASentinelCharacter* HighestPriorityThreat = nullptr;

		TArray<ASentinelCharacter*> ThreatsToRemove;
		for (ASentinelCharacter* Threat : SeenThreats)
		{
			if(!IsValid(Threat))
			{
				ThreatsToRemove.Add(Threat);
				continue;
			}
			
			// Evaluate threat priority using the provided function
			const float ThreatPriority = EvaluateThreatPriority(Threat);

			// Update the highest priority threat
			if (ThreatPriority > HighestPriority)
			{
				HighestPriority = ThreatPriority;
				HighestPriorityThreat = Threat;
			}
		}

		for(const ASentinelCharacter* ToRemove : ThreatsToRemove)
		{
			SeenThreats.Remove(ToRemove);
		}
		// Set the highest priority threat as the target
		if(HighestPriorityThreat != nullptr)
		{
			SetTarget(HighestPriorityThreat);
			return;
		}
	}
	SetDefaultTarget();
}

void ASentinelController::RecalculateThreatToTarget()
{
	if (const UPathFollowingComponent* Pathing = GetPathFollowingComponent())
	{
		float BaseThreat = 2000.0f;  // Adjust this as needed
		ThreatToTarget = BaseThreat / (Pathing->GetRemainingPathCost() + 1.0f);  // Adding 1 to avoid division by zero
	}
}

void ASentinelController::RecalculateThreatToPrincipal()
{
	ThreatToPrincipal = 0;
}

float ASentinelController::EvaluateThreatPriority(ASentinelCharacter* _SentinelCharacter)
{
/*
	*For now they calculate priority by taking the highest threat.
	Influence it with squad threat level?
	Influence it with how many squadmates are targeting the same threat / squad
	Should priority calculation be the directors job?
	No, Sentinels ultimately make the decisions based on their state. Director will change their state depending on current game state.
*/
	if(_SentinelCharacter->GetHealthComponent()->IsOnLastStand()) return TNumericLimits<float>::Min();
	
	float PriorityEvaluation = 0;
	
	const ASentinelSquad* ThreatSquad = _SentinelCharacter->GetSquad();
	const ASentinelSquad* MySquad = NPCBase->GetSquad();
	//const int NrSentinelsAttackingSquad = MySquad->GetNrAttackingSentinels(ThreatSquad); // how many squad members attacking targets squad? // ******** CRASHES IDK Y
	const int NrSentinelsAttackingTarget = MySquad->GetNrAttackingSentinels(_SentinelCharacter); // how many squad members attacking target sentinel
	const int SentinelThreatToPrincipal = EvaluateThreatToPrincipal(_SentinelCharacter);
	//const int ThreatSquadSize = ThreatSquad->GetNrSentinels();


	const float DistanceToTarget = FVector::Dist(_SentinelCharacter->GetActorLocation(), _SentinelCharacter->GetActorLocation());
	const float MaxDistance = 1000.0f; 
	const float DistanceWeight = FMath::Lerp(0.0f, 1.0f, FMath::Clamp(DistanceToTarget / MaxDistance, 0.0f, 10000.0f));
	PriorityEvaluation += DistanceWeight;

	
	// Adjust PriorityEvaluation based on squared distance
	PriorityEvaluation += _SentinelCharacter->GetSentinelController()->GetThreatToTarget();
	PriorityEvaluation += ThreatSquad->GetAverageThreat();
	PriorityEvaluation += SentinelThreatToPrincipal;
	
	PriorityEvaluation -= MySquad->GetAverageThreat() * (NrSentinelsAttackingTarget / MySquad->GetNrSentinels());
	
	return PriorityEvaluation;
}


FVector ASentinelController::CalculateProtectivePos(const ASentinelCharacter* Protectee,
	const ASentinelCharacter* Attacker, float DistanceInFrontOfProtectee)
{
	// Calculate vectors from attacker and protectee
	FVector ProtecteeLocation = Protectee->GetActorLocation();
	FVector AttackerLocation = Attacker->GetActorLocation();

	// Calculate the vector from protectee to attacker
	FVector ProtecteeToAttacker = (AttackerLocation - ProtecteeLocation).GetSafeNormal();

	// Calculate the protective position in between the attacker and protectee
	FVector ProtectivePos = ProtecteeLocation + ProtecteeToAttacker * DistanceInFrontOfProtectee;

	return ProtectivePos;
}

FVector ASentinelController::CalculateSquadAvoidance(const ASentinelSquad* SquadToAvoid)
{
	FVector Avoidance = FVector::Zero();
	for(ASentinelCharacter* SentinelToAvoid : SquadToAvoid->GetSentinels())
	{
		Avoidance += CalculateCharacterAvoidance(SentinelToAvoid);
	}

	return Avoidance;
}

FVector ASentinelController::CalculateCharacterAvoidance(const ASentinelCharacter* ToAvoid)
{
	// Implement logic to calculate a steering force to avoid a specific character
	FVector AvoidanceForce = FVector::ZeroVector;

	// Example: Calculate a force based on the position of the character to avoid
	AvoidanceForce = NPCBase->GetActorLocation() - ToAvoid->GetActorLocation();

	return AvoidanceForce.GetSafeNormal();
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

TSet<ASentinelCharacter*> ASentinelController::GetSeenThreats() const
{
	return SeenThreats;
}

void ASentinelController::SetRole(ERoles toRole)
{
	BlackboardComponent->SetValueAsEnum(FName(BBKeys::Role), static_cast<uint8>(toRole));
}

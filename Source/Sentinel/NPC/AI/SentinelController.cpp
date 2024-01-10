// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/SentinelController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sentinel/NPC/NPCBase.h"
#include "BlackboardKeys.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/SentinelPlayerCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/Actors/SentinelFaction.h"
#include "Steering/BlockThreat.h"
#include "Steering/Follow.h"
#include "Steering/SteeringBehavior.h"


ASentinelController::ASentinelController(const FObjectInitializer& ObjectInitializer)
	: RetargetingInterval(1.0f),
	ThreatUpdateInterval(.5f),
	MoveUpdateInterval(.5f),
	RetargetingIntervalTimer(0.1f),
	ThreatUpdateTimer(0.1f),
	CurrentSteering(nullptr),
	MoveUpdateTimer(0.1f)
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	BlockThreatSteering = CreateDefaultSubobject<UBlockThreat>(TEXT("Threat Blocking Component"));
	BlockThreatSteering->SetBlackboard(BlackboardComponent);
	
	FollowSteering = CreateDefaultSubobject<UFollow>(TEXT("Follow Component"));
	CurrentSteering = BlockThreatSteering;
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

	UpdateMovement(DeltaSeconds);
	UpdateThreatToTargetTimer(DeltaSeconds);
}

void ASentinelController::SetPrincipal(ASentinelCharacter* NewPrincipal) const
{
	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentPrincipal), NewPrincipal);
}

void ASentinelController::SetTargetToKill(ASentinelCharacter* Target) const
{
	BlackboardComponent->SetValueAsObject(FName(BBKeys::TargetToKill), Target);
}

void ASentinelController::SetToFollow()
{
	CurrentSteering = FollowSteering;
}

void ASentinelController::SetToFollow(ASentinelCharacter* ToFollow)
{
	FollowSteering->SetToFollow(ToFollow);
	CurrentSteering = FollowSteering;
}

void ASentinelController::SetTarget(ASentinelCharacter* NewTarget) const
{
	if(auto CurrentTarget = GetCurrentTarget())
	{
		if(CurrentTarget == NewTarget) return;
		CurrentTarget->RemoveTargetedBy(NPCBase);
	}

	BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentTarget), NewTarget);
	NewTarget->AddTargetedBy(NPCBase);
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


float ASentinelController::GetThreatToTarget()  const
{
	if(ASentinelCharacter* Target = GetTarget())
	{
		return GetThreatToSentinel(Target);
	}

	return 0;
}

float ASentinelController::GetThreatToPrincipal() const
{
	return 0;
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

// TODO: Threat should be a location and intensity
ASentinelCharacter* ASentinelController::GetCurrentThreat() const
{
	if(ASentinelCharacter* CurrentThreat = Cast<ASentinelCharacter>(BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentTarget))))
		return CurrentThreat;

	return nullptr;
}

ASentinelCharacter* ASentinelController::GetCurrentTarget() const
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

void ASentinelController::UpdateMovement(float DeltaTime)
{
	if(!CurrentSteering) return;
	const FVector Steering = CurrentSteering->CalculateSteering(NPCBase);
	FVector TargetLocation = NPCBase->GetActorLocation() + Steering;
	if(FVector::DistSquared(NPCBase->GetActorLocation(), TargetLocation) > 200.0f)
	MoveTo(TargetLocation);
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
//
// float ASentinelController::EvaluateThreatToTarget()
// {
// 	if (const ASentinelCharacter* Principal = GetPrincipal())
// 	{
// 		// Get the AI controller's path-following component
// 		const UPathFollowingComponent* PathComponent = GetPathFollowingComponent();
//
// 		if (PathComponent)
// 		{
// 			// Get the remaining path length from the movement component
// 			float RemainingPathLength = PathComponent->GetRemainingPathCost();
//
// 			// Normalize the distance based on some reference value (e.g., maximum possible distance)
// 			const float MaxPossibleDistance = 1000.0f; // Adjust as needed
// 			float NormalizedDistance = FMath::Clamp(RemainingPathLength / MaxPossibleDistance, 0.0f, 1.0f);
//
// 			// Calculate health ratio (normalized between 0 and 1)
// 			const float HealthRatio = NPCBase->GetHealthComponent()->GetHealth() / NPCBase->GetHealthComponent()->
// 				GetMaxHealth();
//
// 			// Weight for each factor (adjust based on importance)
// 			constexpr float DistanceWeight = 0.5f;
// 			constexpr float HealthWeight = 0.5f;
//
// 			// Calculate overall threat score
// 			const float ThreatScore = (NormalizedDistance * DistanceWeight) + (HealthRatio * HealthWeight);
//
// 			return ThreatScore;
// 		}
// 	}
//
// 	return 0;
// }
//

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
	if(ASentinelCharacter* Target = GetTarget())
	{
		ThreatToTarget = GetThreatToSentinel(Target);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Target set"));
	}
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
	const float DistanceWeight = FMath::Lerp(10000.0f, 0.0f, FMath::Clamp(DistanceToTarget / MaxDistance, 0.0f, 10000.0f));
	PriorityEvaluation += DistanceWeight;

	
	// Adjust PriorityEvaluation based on squared distance
	if(Cast<ASentinelPlayerCharacter>(_SentinelCharacter))
	{
		constexpr float PlayerThreatLevel = 9001.0f; // !!!!
		PriorityEvaluation += PlayerThreatLevel;
	}
	else
	{
		PriorityEvaluation += _SentinelCharacter->GetSentinelController()->GetThreatToTarget();
		PriorityEvaluation += ThreatSquad->GetAverageThreat();
	}
	
	PriorityEvaluation += SentinelThreatToPrincipal;
	
	PriorityEvaluation -= MySquad->GetAverageThreat() * (NrSentinelsAttackingTarget / MySquad->GetNrSentinels());
	
	return PriorityEvaluation;
}


FVector ASentinelController::CalculateProtectivePos(const ASentinelCharacter* Protectee,
	const ASentinelCharacter* Attacker, float DistanceInFrontOfProtectee) const
{
	// Calculate vectors from attacker and protectee
	const FVector ProtecteeLocation = Protectee->GetActorLocation();
	const FVector AttackerLocation = Attacker->GetActorLocation();

	// Calculate the vector from protectee to attacker
	const FVector ProtecteeToAttacker = (AttackerLocation - ProtecteeLocation).GetSafeNormal();

	// Calculate the protective position in between the attacker and protectee
	const FVector ProtectivePos = ProtecteeLocation + ProtecteeToAttacker * DistanceInFrontOfProtectee;

	return ProtectivePos;
}

FVector ASentinelController::CalculateSquadAvoidance(const ASentinelSquad* SquadToAvoid) const
{
	FVector Avoidance = FVector::Zero();
	for(const ASentinelCharacter* SentinelToAvoid : SquadToAvoid->GetSentinels())
	{
		Avoidance += CalculateCharacterAvoidance(SentinelToAvoid);
	}

	return Avoidance;
}

FVector ASentinelController::CalculateCharacterAvoidance(const ASentinelCharacter* ToAvoid) const
{
	// Implement logic to calculate a steering force to avoid a specific character
	const FVector AvoidanceForce = NPCBase->GetActorLocation() - ToAvoid->GetActorLocation();


	return AvoidanceForce.GetSafeNormal();
}

void ASentinelController::SetBlockThreatSteering()
{
	CurrentSteering = BlockThreatSteering;
}

void ASentinelController::SetFollow(ASentinelCharacter* toFollow)
{
	if(toFollow)
		FollowSteering->SetToFollow(toFollow);

	CurrentSteering = FollowSteering;
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

ASentinelCharacter* ASentinelController::GetTargetToKill() const
{
	return Cast<ASentinelCharacter>(
	BlackboardComponent->GetValueAsObject(FName(BBKeys::TargetToKill)));
}

TSet<ASentinelCharacter*> ASentinelController::GetSeenThreats() const
{
	return SeenThreats;
}

ASentinelCharacter* ASentinelController::GetHighestThreat() 
{
	if(SeenThreats.IsEmpty()) return nullptr;
	
	ASentinelCharacter* HighestThreat = *SeenThreats.begin();

	for(ASentinelCharacter* Threat : SeenThreats)
	{
		if(ASentinelController* SController = Threat->GetSentinelController())
		if(ASentinelCharacter* Sentinel = SController->GetHighestThreat())
		{
			return Sentinel;
		}
	}

	return nullptr;
}

void ASentinelController::SetRole(ERoles toRole) const
{
	BlackboardComponent->SetValueAsEnum(FName(BBKeys::Role), static_cast<uint8>(toRole));
}

ERoles ASentinelController::GetRole() const
{
	return static_cast<ERoles>(BlackboardComponent->GetValueAsEnum(FName(BBKeys::Role)));
}

UFollow* ASentinelController::GetFollowComponent() const
{
	return FollowSteering;
}

UBlockThreat* ASentinelController::GetThreatBlockingComponent() const
{
	return BlockThreatSteering;
}

bool ASentinelController::TrySetEscort(ASentinelCharacter* ToEscort) const
{
	const ERoles CurrentRole = static_cast<ERoles>(BlackboardComponent->GetValueAsEnum(FName(BBKeys::Role)));

	if(ToEscort == GetPrincipal()) return false; // Is already escorting, doesn't count as setting.
	
	// Lambda function for setting the escort role
	auto SetEscortRole = [this, ToEscort]() {
		SetRole(ERoles::Escort);
		SetPrincipal(ToEscort);
	};
	
	if (CurrentRole == ERoles::Escort)
	{
		const float ProtectionToCurrentPrincipal = GetProtectionToSentinel(GetPrincipal());
		const float ProtectionToNewPrincipal = GetProtectionToSentinel(ToEscort);

		// If I can protect the new sentinel better than I can protect the current one do that
		if(ProtectionToNewPrincipal > ProtectionToCurrentPrincipal)
		{
			SetEscortRole();
			return true;
		}
		
		return false;
	}

	// If the sentinel is not already set as an Escort, proceed with setting the role
	SetEscortRole();

	return true;
}

bool ASentinelController::TrySetKiller(ASentinelCharacter* Target) const
{
	const ERoles CurrentRole = static_cast<ERoles>(BlackboardComponent->GetValueAsEnum(FName(BBKeys::Role)));
	
	if(Target == GetTarget()) return false; // Is already escorting, doesn't count as setting.

	SetRole(ERoles::Killer);
	SetTargetToKill(Target);

	return true;
}

float ASentinelController::GetProtectionToSentinel(ASentinelCharacter* Sentinel) const
{
	if(!Sentinel) return -1;
	
	float ProtectionToPrincipal = 0;
	
	if (const UPathFollowingComponent* Pathing = GetPathFollowingComponent())
	{
		if(FVector::DistSquared(Pathing->GetPathDestination(), Sentinel->GetActorLocation()) < 100.0f * 100.0f)
		{
			constexpr float ProtectionDistanceThreshold  = 2000.0f;  // Adjust this as needed
			constexpr float HealthWeight = 100.f;

			ProtectionToPrincipal = (Pathing->GetRemainingPathCost() + 1.0f) / ProtectionDistanceThreshold;
			ProtectionToPrincipal += NPCBase->GetHealthComponent()->GetHealth() * HealthWeight;
		}
	}

	return ProtectionToPrincipal;
}

float ASentinelController::GetThreatToSentinel(ASentinelCharacter* Sentinel) const
{
	float ThreatLvl = 0;
	
	if (const UPathFollowingComponent* Pathing = GetPathFollowingComponent())
	{
		constexpr float BaseThreat = 1000.0f;  // Adjust this as needed

		ThreatLvl = BaseThreat / (Pathing->GetRemainingPathCost() + 1.0f);  // Adding 1 to avoid division by zero
		//ThreatLvl += NPCBase->GetHealthComponent()->GetHealth() * HealthWeight;
	}



	UE_LOG(LogTemp, Log, TEXT("[GetThreatToSentinel] %s ThreatLvl: %f"), *NPCBase->GetName(), ThreatLvl);
	
	return ThreatLvl;
}

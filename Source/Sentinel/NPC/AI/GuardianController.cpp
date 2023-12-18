// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/GuardianController.h"

#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/ZombSentinel.h"

AGuardianController::AGuardianController(FObjectInitializer const& objectInit)
{
}

void AGuardianController::OnSeePawn(APawn* SeenPawn)
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
			// Immediately attack if its a zomb (temp)
			if(Cast<AZombSentinel>(Sentinel)) 
			{
				AddThreat(Sentinel);
				SetTarget(Sentinel);
			} else
			{
				int SSFactionIdx = Sentinel->GetFactionIdx();
				// int SSSquadIdx = Sentinel->GetSquad();

				if (SSFactionIdx != NPCBase->GetFactionIdx())
				{
					AddThreat(Sentinel);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Seen unknown pawn"));
		}
	}
}

void AGuardianController::OnLastStand()
{
	DisableBehaviorTree();
	NPCBase->GetSquad()->RequestMedic(NPCBase);
}

void AGuardianController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(NPCBase->GetHealthComponent()->GetHealth() <= 0) return;

	
	UpdateRetargetingTimer(DeltaSeconds);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/ZombController.h"

#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/GuardianSentinel.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/ZombSentinel.h"

void AZombController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(NPCBase->GetHealthComponent()->GetHealth() <= 0) return;

	UpdateRetargetingTimer(DeltaSeconds * .5f);
}

void AZombController::OnSeePawn(APawn* SeenPawn)
{
	if (ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(SeenPawn))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(SeenPawn->GetController()))
		{
			AddThreat(Sentinel);
		}
		else if (Cast<ANPCBase>(Sentinel))
		{
			// Immediately attack if its a zomb (temp)
			if(Cast<AGuardianSentinel>(Sentinel)) 
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

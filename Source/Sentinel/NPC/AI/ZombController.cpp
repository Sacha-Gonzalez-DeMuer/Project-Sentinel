// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/ZombController.h"

#include "Sentinel/SentinelPlayerCharacter.h"
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
		if (ASentinelPlayerCharacter* PlayerController = Cast<ASentinelPlayerCharacter>(Sentinel))
		{
			// Seen player!
			UE_LOG(LogTemp, Log, TEXT("Zomb Seen player!"));
			AddThreat(Sentinel);
		}
		else if (!NPCBase->IsAlly(Sentinel))
		{
			AddThreat(Sentinel);
		}
		else
		{
		}
	}
}

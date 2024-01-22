// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/GuardianController.h"

#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/RecruitingArea.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/GuardianSentinel.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/ZombSentinel.h"
#include "Steering/Follow.h"

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
		else if (!NPCBase->IsAlly(Sentinel))
		{
			AddThreat(Sentinel);
		}
		else if (AGuardianSentinel* Guardian = Cast<AGuardianSentinel>(SeenPawn))
		{
			if(ARecruitingArea* RecArea = Guardian->GetRecruitingArea())
			{
				if(ASentinelSquad* Squad = NPCBase->GetSquad())
				{
					Squad->AddSeenRecruitArea(RecArea);
				}
			}
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

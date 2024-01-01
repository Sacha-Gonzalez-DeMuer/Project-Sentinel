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
		else if (!NPCBase->IsAlly(Sentinel))
		{
			AddThreat(Sentinel);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Guardian %s saw an unknown Sentinel: %s"), *NPCBase->GetName(), *Sentinel->GetName());
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

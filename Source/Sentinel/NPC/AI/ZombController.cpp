// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/ZombController.h"

#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/NPCBase.h"

void AZombController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(NPCBase->GetHealthComponent()->GetHealth() <= 0) return;

	UpdateRetargetingTimer(DeltaSeconds * .5f);
}

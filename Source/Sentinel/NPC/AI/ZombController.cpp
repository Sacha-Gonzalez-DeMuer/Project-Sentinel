// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/ZombController.h"

void AZombController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateThreatToTargetTimer(DeltaSeconds);
	UpdateRetargetingTimer(DeltaSeconds * .5f);
}

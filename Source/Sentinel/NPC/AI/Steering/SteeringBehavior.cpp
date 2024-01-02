// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Steering/SteeringBehavior.h"


USteeringBehavior::USteeringBehavior()
{
}

FVector USteeringBehavior::CalculateSteering(const ASentinelCharacter* SteeringAgent)
{
	UE_LOG(LogTemp, Log, TEXT("Calculating BASECLASS steering of: %s"), *GetName());
	return {0,0,0};
}

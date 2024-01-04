// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Actors/SentinelFaction.h"

#include "SentinelSquad.h"

// Sets default values
ASentinelFaction::ASentinelFaction()
	: FactionIdx(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASentinelFaction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASentinelFaction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASentinelFaction::SetFactionIdx(int idx) 
{
	FactionIdx = idx;
}


int ASentinelFaction::GetFactionIdx() const
{
	return FactionIdx;
}

ASentinelSquad* ASentinelFaction::GetSquad(int SquadIdx)
{
	if(SquadIdx < Squads.Num() && IsValid(Squads[SquadIdx]) && Squads[SquadIdx] != nullptr) return Squads[SquadIdx];

	// Create a new Squad
	if (ASentinelSquad* NewSquad = GetWorld()->SpawnActor<ASentinelSquad>(SquadType))
	{
		// Initialize the Squad
		NewSquad->SetIdx(FactionIdx, SquadIdx);

		// Ensure the array is large enough
		Squads.SetNum(FMath::Max(Squads.Num(), SquadIdx + 1));

		// Add the new Squad at the specified index
		Squads[SquadIdx] = NewSquad;

		UE_LOG(LogTemp, Log, TEXT("Squad Created"));
		return NewSquad;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to Create new Squad"));
	return nullptr;
}

void ASentinelFaction::SetSquad(ASentinelSquad* Squad)
{
	if(Squad->GetFactionIdx() != FactionIdx)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ASentinelFaction::SetSquad] Stopped Squad from trying do join opposing faction, Was this intended?."));
		return;
	}
	
	// Get the squad index
	const int SquadIdx = Squad->GetSquadIdx();

	// Check if SquadIdx is a valid index in the Squads array
	if (SquadIdx >= 0 && SquadIdx < Squads.Num())
	{
		// Set the squad in the array
		Squads[SquadIdx] = Squad;
	}
	else
	{
		// Ensure the array is large enough
		Squads.SetNum(FMath::Max(Squads.Num(), SquadIdx + 1));

		// Add the new Squad at the specified index
		Squads[SquadIdx] = Squad;
	}
}

TArray<ASentinelSquad*> ASentinelFaction::GetSquads() const
{
	return Squads;
}




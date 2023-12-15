// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/SentinelDirector.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelFaction.h"
#include "Sentinel/Actors/SentinelSquad.h"

// Sets default values
ASentinelDirector::ASentinelDirector()
	: MaxFactions(2), MaxSquads(4)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}


// Called when the game starts or when spawned
void ASentinelDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASentinelDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASentinelDirector::AddSentinel(ASentinelCharacter* Sentinel)
{
	int FactionIdx = Sentinel->GetFaction();
	int SquadIdx = Sentinel->GetSquad();

	ASentinelFaction* Faction = Factions[FactionIdx];
	if(!Faction)
	{
		Faction = CreateFaction(FactionIdx);
		Faction->GetSquad(SquadIdx)->AddSentinel(Sentinel);
	}
}

ASentinelFaction* ASentinelDirector::CreateFaction(int FactionIdx)
{
	// Check if the faction already exists
	for (ASentinelFaction* Faction : Factions)
	{
		if (Faction->GetFactionIdx() == FactionIdx)
		{
			// Faction already exists, no need to create a new one
			return Faction;
		}
	}

	// Create a new faction
	ASentinelFaction* NewFaction = GetWorld()->SpawnActor<ASentinelFaction>(ASentinelFaction::StaticClass());
	if (NewFaction)
	{
		// Initialize the faction
		NewFaction->SetFactionIdx(FactionIdx);

		// Ensure the array is large enough
		Factions.SetNum(FMath::Max(Factions.Num(), FactionIdx + 1));

		// Add the new faction at the specified index
		Factions[FactionIdx] = NewFaction;

		UE_LOG(LogTemp, Log, TEXT("Faction Created"));
		return NewFaction;
	}

	return nullptr;
}

ASentinelSquad* ASentinelDirector::GetSquad(int FactionIdx, int SquadIdx)
{
	if (Factions.IsValidIndex(FactionIdx))
	{
		return Factions[FactionIdx]->GetSquad(SquadIdx);
	}
	else
	{
		// create faction?
		return nullptr;
	}
}


//Squad* ASentinelDirector::GetSquad(int FactionIdx, int SquadIdx) 
//{
// // Check if the specified squad exists in the faction's squads map
//if (Squad* Squad = Factions.Find(FactionIdx)
//{
// // Use const_cast to remove const qualifier since we want to modify squad members states
//	return Squad;
//}

//// If the faction or squad is not found, return nullptr
//	return nullptr;
//}

//void ASentinelDirector::AddSentinel(int FactionIdx, int SquadIdx, ASentinelCharacter* Sentinel)
//{
//	// Try to find the faction
//	FSquad* SentinelSquad = Factions[FactionIdx][SquadIdx];
//
//	// If the faction doesn't exist, create a new one
//	if (!SentinelSquad)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Squad not initialized. FactionIdx: %d"), SquadIdx);
//		return;
//	}
//
//	SentinelSquad->Members.Add(Sentinel);
//}

//
// FSquad* ASentinelDirector::CreateSquad(int FactionIdx, int SquadIdx)
// {
// 	FSquad NewSquad;
// 	NewSquad.FactionIdx = FactionIdx;
// 	NewSquad.SquadIdx = SquadIdx;
//
// 	if(Factions.Contains(FactionIdx))
// 		Factions.Find(FactionIdx)->Add(SquadIdx, NewSquad);
// 	else
// 		CreateFaction(FactionIdx, SquadIdx);
//
// 	return &NewSquad;
// }
//
// Faction* ASentinelDirector::CreateFaction(int FactionIdx)
// {
// 	FSquad NewSquad;
// 	NewSquad.FactionIdx = FactionIdx;
// 	NewSquad.SquadIdx = 0;
// 	
// 	Faction NewFaction;
// 	NewFaction.Add(FactionIdx, NewSquad);
// 	Factions.Add(FactionIdx, NewFaction);
//
// 	return &NewFaction;
// }
//
// void ASentinelDirector::CreateFaction(int FactionIdx, int SquadIdx)
// {
// 	// default squad
// 	Faction NewFaction;
// 	
// 	FSquad NewSquad;
// 	NewSquad.FactionIdx = FactionIdx;
// 	NewSquad.SquadIdx = 0;
// 	NewFaction.Add(FactionIdx, NewSquad);
// 	
// 	NewSquad.FactionIdx = FactionIdx;
// 	NewSquad.SquadIdx = SquadIdx;
// 	NewFaction.Add(FactionIdx, NewSquad);
//
// 	
// 	Factions.Add(FactionIdx, NewFaction);
// }

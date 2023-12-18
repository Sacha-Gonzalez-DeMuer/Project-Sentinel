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
    UE_LOG(LogTemp, Log, TEXT(" SQUAD"))
    
    // 1. Ensure that Sentinel is not nullptr
    if (!Sentinel)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sentinel is nullptr. Cannot proceed."));
        return;
    }

    int FactionIdx = Sentinel->GetFactionIdx();
    int SquadIdx = Sentinel->GetSquadIdx();

    // 2. Ensure that FactionIdx is non-negative
    if (FactionIdx < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid FactionIdx (%d). Cannot proceed."), FactionIdx);
        return;
    }

    // 3. Ensure that SquadIdx is non-negative
    if (SquadIdx < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid SquadIdx (%d). Cannot proceed."), SquadIdx);
        return;
    }

    // Ensure the array is large enough
    // 4. Ensure Factions.Num() is non-negative before taking its maximum
    int32 NewArraySize = FMath::Max(Factions.Num(), FactionIdx + 1);
    if (NewArraySize < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid array size. Cannot proceed."));
        return;
    }
    Factions.SetNum(NewArraySize, false);

    // 5. Ensure FactionIdx is within the bounds of the Factions array
    if (FactionIdx >= Factions.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("FactionIdx (%d) out of bounds. Cannot proceed."), FactionIdx);
        return;
    }

    ASentinelFaction* Faction = Factions[FactionIdx];
    if (!Faction)
    {
        // 6. Handle the case where CreateFaction returns nullptr
        Faction = CreateFaction(FactionIdx);
        if (!Faction)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create Faction. Cannot proceed."));
            return;
        }
    }

    auto Squad = Faction->GetSquad(SquadIdx);

    if (Squad)
    {
        UE_LOG(LogTemp, Log, TEXT("GOT SQUAD"))
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("WHERE D SQUAD"));
    }

    // 7. Ensure Squad is not nullptr before calling AddSentinel
    if (Squad)
    {
        Squad->AddSentinel(Sentinel);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Squad is nullptr. Cannot add Sentinel."));
    }
}
ASentinelFaction* ASentinelDirector::CreateFaction(int FactionIdx)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Faction %d"), FactionIdx);

	// 1. Ensure that FactionIdx is non-negative
	if (FactionIdx < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid FactionIdx (%d). Cannot create faction."), FactionIdx);
		return nullptr;
	}

	// Check if the faction already exists
	// 2. Ensure FactionIdx is within the bounds of the Factions array
	if (FactionIdx < Factions.Num() && Factions[FactionIdx] != nullptr)
	{
		return Factions[FactionIdx];
	}

	// Create a new faction
	ASentinelFaction* NewFaction = GetWorld()->SpawnActor<ASentinelFaction>(FactionClass);
	if (NewFaction)
	{
		// Initialize the faction
		NewFaction->SetFactionIdx(FactionIdx);

		// Ensure the array is large enough
		// 3. Ensure Factions.Num() is non-negative before taking its maximum
		int32 NewArraySize = FMath::Max(Factions.Num(), FactionIdx + 1);
		Factions.SetNum(NewArraySize);

		// 4. Ensure FactionIdx is within the bounds of the Factions array after resizing
		if (FactionIdx >= Factions.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("FactionIdx (%d) out of bounds after resizing. Cannot proceed."), FactionIdx);
			return nullptr;
		}

		// Add the new faction at the specified index
		Factions[FactionIdx] = NewFaction;

		UE_LOG(LogTemp, Log, TEXT("Faction Created"));
		return NewFaction;
	}

	// Log more details about the spawning failure
	UE_LOG(LogTemp, Warning, TEXT("Failed to spawn ASentinelFaction. Class: %s, Location: %s, Rotation: %s"),
		*FactionClass->GetName(),
		*GetActorLocation().ToString(),
		*GetActorRotation().ToString()
	);
	
	return nullptr;
}
ASentinelSquad* ASentinelDirector::GetSquad(int FactionIdx, int SquadIdx)
{
	if(FactionIdx < Factions.Num())
	{
		if(!IsValidLowLevel()) return nullptr;
		if(!Factions[FactionIdx]->IsValidLowLevel()) return nullptr;
		
		if (IsValid(Factions[FactionIdx]))
		{
			// Return the squad based on the provided squad index
			ASentinelSquad* Squad = Factions[FactionIdx]->GetSquad(SquadIdx);
			
			return Squad;
		}
	}
	else
	{
		// Log information for debugging
		ASentinelFaction* NewFaction = CreateFaction(FactionIdx);
		ASentinelSquad* NewSquad = NewFaction->GetSquad(SquadIdx);
		
		return NewSquad;
	}

	return nullptr;
}

ASentinelFaction* ASentinelDirector::GetFaction(int FactionIdx)
{
	if (FactionIdx >= 0 && FactionIdx < Factions.Num())
	{
		return Factions[FactionIdx];
	}
	return nullptr;
}

void ASentinelDirector::RequestAssistance(ASentinelSquad* Threat)
{
	// Check if the threat is valid
	if (!Threat)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid threat provided for assistance request."));
		return;
	}

	// Assess the threat level (you can implement your own logic for this)
	float ThreatLevel = Threat->GetAverageThreat();
	//
	// // Find a suitable squad to assist based on threat level
	// ASentinelSquad* AvailableSquad = nullptr;
	//
	// for (ASentinelFaction* Faction : Factions)
	// {
	// 	for (ASentinelSquad* Squad : Faction->GetSquads())
	// 	{
	// 		// You can customize this condition based on your game's logic
	// 		//if (Squad->IsAvailableForAssistance() && Squad->EvaluateThreatLevel() < ThreatLevel)
	// 		//{
	// 		//	AvailableSquad = Squad;
	// 		//	break;
	// 		//}
	// 	}
	//
	// 	if (AvailableSquad)
	// 	{
	// 		//// Assign the squad to assist the threat
	// 		//AvailableSquad->AssistThreat(Threat);
	// 		break;
	// 	}
	// }
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

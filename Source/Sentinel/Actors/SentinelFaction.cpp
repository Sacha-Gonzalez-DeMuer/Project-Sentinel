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
	if(SquadIdx < Squads.Num() && Squads[SquadIdx] != nullptr) return Squads[SquadIdx];

	// Create a new Squad
	ASentinelSquad* NewSquad = GetWorld()->SpawnActor<ASentinelSquad>(SquadType);
	if (NewSquad)
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

	return nullptr;
	
}




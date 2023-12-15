// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Actors/SentinelFaction.h"

#include "SentinelSquad.h"

// Sets default values
ASentinelFaction::ASentinelFaction()
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
	if (Squads.IsValidIndex(SquadIdx))
	{
		return Squads[SquadIdx];
	}
	else
	{
		// Ensure the array is large enough to accommodate the new squad
		Squads.SetNum(FMath::Max(Squads.Num(), SquadIdx + 1));

		// Create a new squad
		ASentinelSquad* NewSquad = GetWorld()->SpawnActor<ASentinelSquad>(ASentinelSquad::StaticClass());

		// Initialize the new squad with SquadIdx or other data
		NewSquad->SetIdx(FactionIdx, SquadIdx);

		// Assign the new squad to the array
		Squads[SquadIdx] = NewSquad;
		UE_LOG(LogTemp, Log, TEXT("Squad Created"));

		return NewSquad;
	}
}




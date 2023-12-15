// Fill out your copyright notice in the Description page of Project Settings.


#include "SentinelSquad.h"

// Sets default values
ASentinelSquad::ASentinelSquad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASentinelSquad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASentinelSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASentinelSquad::AddSentinel(ASentinelCharacter* Sentinel)
{
	if(Sentinels.Contains(Sentinel)) return;
	Sentinels.Add(Sentinel);
}

void ASentinelSquad::SetIdx(int Faction, int Squad)
{
	FactionIdx = Faction;
	SquadIdx = Squad;
}


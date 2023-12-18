// Fill out your copyright notice in the Description page of Project Settings.


#include "SentinelSquad.h"

#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"

// Sets default values
ASentinelSquad::ASentinelSquad()
	: FactionIdx(0), SquadIdx(0), Faction(nullptr)
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

void ASentinelSquad::SetIdx(int NewFactionIdx, int NewSquadIdx)
{
	FactionIdx = NewFactionIdx;
	SquadIdx = NewSquadIdx;
}

void ASentinelSquad::SetFaction(ASentinelFaction* NewFaction)
{
	Faction = NewFaction;
}

float ASentinelSquad::GetAverageThreat() const
{
	float TotalThreat = 0;
	for(const ASentinelCharacter* Sentinel : Sentinels)
	{
		if(!Sentinel->IsValidLowLevel())
			continue;

		if(ASentinelController* SController = Sentinel->GetSentinelController())
			if(SController->IsValidLowLevel())
			{
				TotalThreat += Sentinel->GetSentinelController()->GetThreatToTarget();
			}
		else UE_LOG(LogTemp, Error, TEXT("SentinelSquad::GetAverageThreat. Invalid Controller"));
	}

	return TotalThreat / Sentinels.Num();
}

void ASentinelSquad::LeaveSquad(ASentinelCharacter* SentinelToLeave)
{
	Sentinels.Remove(SentinelToLeave);
}


int ASentinelSquad::GetNrAttackingSentinels(const ASentinelCharacter* Target) const
{
	int nrAttackers = 0;

	// Check if the Target pointer is valid
	if (Target)
	{
		//TArray<ASentinelCharacter*> ToRemove;
		
		for (ASentinelCharacter* SquadSentinel : Sentinels)
		{
			// Check if SquadSentinel pointer is valid
			if (SquadSentinel)
			{
				// Check if the SentinelController pointer is valid
				ASentinelController* SentinelController = SquadSentinel->GetSentinelController();
				if (SentinelController)
				{
					// Check if the target of the SentinelController matches the provided Target
					if (SentinelController->GetTarget() == Target)
					{
						++nrAttackers;
					}
				}
				else
				{
					// Log if the SentinelController is null
					UE_LOG(LogTemp, Warning, TEXT("GetNrAttackingSentinels: SentinelController for SquadSentinel is null."));
//					ToRemove.Add(SquadSentinel);
				}
			}
			else
			{
				// Log if SquadSentinel is null
				UE_LOG(LogTemp, Warning, TEXT("GetNrAttackingSentinels: SquadSentinel in Sentinels array is null."));
			}
		}

		//for(ASentinelCharacter* SentinelToRemove : ToRemove)
		//{
		//	Sentinels.Remove(SentinelToRemove);
		//}
	}
	else
	{
		// Log if the Target pointer is null
		UE_LOG(LogTemp, Warning, TEXT("GetNrAttackingSentinels: Target pointer is null."));
	}

	return nrAttackers;
}

int ASentinelSquad::GetNrAttackingSentinels(const ASentinelSquad* Target) const
{
	int nrAttackers = 0;
	for(const ASentinelCharacter* SquadMember : Sentinels)
	{
		if(SquadMember->IsValidLowLevel())
		{
			if(ASentinelCharacter* SquadMemberTarget = SquadMember->GetSentinelController()->GetTarget())
			{
				if(SquadMemberTarget->IsValidLowLevel())
				{
					if(ASentinelSquad* SquadMemberTargetSquad = SquadMemberTarget->GetSquad())
					{
						if(SquadMemberTargetSquad == Target)
							++nrAttackers;
					}
				}
			}
		}
	}
	
	return nrAttackers;
}

int ASentinelSquad::GetNrSentinels() const
{
	return Sentinels.Num();
}

TArray<ASentinelCharacter*> ASentinelSquad::GetSentinels() const
{
	return Sentinels;
}

void ASentinelSquad::RequestMedic(ASentinelCharacter* Patient)
{
	for (ASentinelCharacter* Sentinel : Sentinels)
	{
		if(Sentinel == Patient) continue;
		
		if (ASentinelController* SentinelController = Sentinel->GetSentinelController())
		{
			// Set the role and principal for the medic
			SentinelController->SetRole(ERoles::Medic);
			SentinelController->SetPrincipal(Patient);

			// Print that a medic was found and its name
			UE_LOG(LogTemp, Warning, TEXT("Medic Found: %s"), *Sentinel->GetName());
			return;
		}
	}

	// If no medic was found, print a message
	UE_LOG(LogTemp, Warning, TEXT("No Medic Found in the Squad"));
}
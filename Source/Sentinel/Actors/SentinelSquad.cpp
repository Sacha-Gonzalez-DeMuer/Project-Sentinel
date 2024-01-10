// Fill out your copyright notice in the Description page of Project Settings.


#include "SentinelSquad.h"

#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"
#include "Sentinel/NPC/AI/SquadDirectorController.h"
#include "Sentinel/NPC/SentinelDirector.h"

// Sets default values
ASentinelSquad::ASentinelSquad()
	: FactionIdx(0), SquadIdx(0), DefaultPrincipal(nullptr), Faction(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASentinelSquad::BeginPlay()
{
	Super::BeginPlay();

	if(DefaultPrincipal)
		SetPrincipal(DefaultPrincipal);

	if(SentinelDirector)
		SentinelDirector->AddSquad(this);
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

int ASentinelSquad::GetSquadIdx() const
{
	return SquadIdx;
}

int ASentinelSquad::GetFactionIdx() const
{
	return FactionIdx;
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
				}
			}
			else
			{
				// Log if SquadSentinel is null
				UE_LOG(LogTemp, Warning, TEXT("GetNrAttackingSentinels: SquadSentinel in Sentinels array is null."));
			}
		}
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

TArray<ASentinelCharacter*> ASentinelSquad::GetSeenThreats() const
{
	TArray<ASentinelCharacter*> SquadSeenThreats;

	for (const ASentinelCharacter* Sentinel : GetSentinels())
	{
		if (const ASentinelController* SentinelController = Sentinel->GetSentinelController())
		{
			TSet<ASentinelCharacter*> SeenThreats = SentinelController->GetSeenThreats();
			SquadSeenThreats.Append(SeenThreats.Array());
		}
	}

	return SquadSeenThreats;
}

void ASentinelSquad::SetPrincipal(ASentinelCharacter* Principal)
{
	if(ASquadDirectorController* SquadDirector = GetSquadDirector())
	{
		SquadDirector->SetPrincipal(Principal);
	} else UE_LOG(LogTemp, Warning, TEXT("[ASentinelSquad::SetPrincipal] Squad has no director"));
}

bool ASentinelSquad::RequestMedic(ASentinelCharacter* Patient)
{
	UE_LOG(LogTemp, Warning, TEXT("Searching medic"));
	
	for (const ASentinelCharacter* Sentinel : Sentinels)
	{
		if(Sentinel == Patient) continue;
		
		if (const ASentinelController* SentinelController = Sentinel->GetSentinelController())
		{
			// Set the role and principal for the medic
			SentinelController->SetRole(ERoles::Medic);
			SentinelController->SetPrincipal(Patient);

			// Print that a medic was found and its name
			UE_LOG(LogTemp, Warning, TEXT("Medic Found: %s"), *Sentinel->GetName());
			return true;
		}
	}

	// If no medic was found, print a message
	UE_LOG(LogTemp, Warning, TEXT("No Medic Found in the Squad"));

	return false;
	
}

bool ASentinelSquad::RequestEscort(ASentinelCharacter* ToEscort)
{
	for (const ASentinelCharacter* Sentinel : Sentinels)
	{
		if(Sentinel == ToEscort) continue;
		
		if (const ASentinelController* SentinelController = Sentinel->GetSentinelController())
		{
			if(SentinelController->TrySetEscort(ToEscort))
			{
				// Print that a medic was found and its name
				UE_LOG(LogTemp, Warning, TEXT("Escort Found: %s"), *Sentinel->GetName());
				return true;
			}
		}
	}

	// If no medic was found, print a message
	UE_LOG(LogTemp, Warning, TEXT("No Escort Found in the Squad"));
	return false;
}


bool ASentinelSquad::RequestKiller(ASentinelCharacter* ToKill, int NrKillers)
{
	int nrKillersFound = 0;
	for (const ASentinelCharacter* Sentinel : Sentinels)
	{
		if(Sentinel == ToKill) continue;
		
		if (const ASentinelController* SentinelController = Sentinel->GetSentinelController())
		{
			if(SentinelController->TrySetKiller(ToKill))
			{
				// Print that a medic was found and its name
				UE_LOG(LogTemp, Warning, TEXT("Killer Found: %s"), *Sentinel->GetName());

				++nrKillersFound;
				if(nrKillersFound >= NrKillers) return true;
			}
		}
	}

	
	UE_LOG(LogTemp, Warning, TEXT("No Killer Found in the Squad"));
	return false;
}

float ASentinelSquad::CalculatePressure(ASentinelCharacter* Principal) const
{
	// Implement your logic to calculate the total pressure based on SquadSeenThreats
	float TotalPressure = -1.0f;
	for(const ASentinelCharacter* Threat : GetSeenThreats())
	{
		if(const ASentinelController* ThreatController = Threat->GetSentinelController())
			if(ThreatController->GetTarget() == Principal)
			{
				TotalPressure += ThreatController->GetThreatToSentinel(Principal);
			}
	}

	for(const ASentinelCharacter* Sentinel : Sentinels)
	{
		if(const ASentinelController* SentinelController = Sentinel->GetSentinelController())
		{
			if(SentinelController->GetPrincipal() == Principal)
			{
				TotalPressure -= SentinelController->GetProtectionToSentinel(Principal);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[CalculatePressure] Pressure to %s: %f"), *Principal->GetName(), TotalPressure);
	return TotalPressure;
}

ASquadDirectorController* ASentinelSquad::GetSquadDirector() const
{
	if(!IsValidLowLevel())
		return nullptr;
	
	AController* AIController = GetController();
	if (ASquadDirectorController* SController = Cast<ASquadDirectorController>(AIController))
	{
		return SController;
	}
	
	return nullptr;
}

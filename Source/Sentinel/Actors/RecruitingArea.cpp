// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Actors/RecruitingArea.h"

#include "Components/BoxComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/SentinelPlayerCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h" 
#include "Sentinel/NPC/AI/Steering/Follow.h"
#include "Sentinel/Actors/SentinelSquad.h"

// Sets default values
ARecruitingArea::ARecruitingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>("Trigger Area");
}

// Called when the game starts or when spawned
void ARecruitingArea::BeginPlay()
{
	Super::BeginPlay();

	for(ASentinelCharacter* Sentinel : Sentinels)
	{
		Sentinel->SetRecruitArea(this);
	}
}

// Called every frame
void ARecruitingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(RecruitTimer < TimeToRecruit)
	{
		RecruitTimer += DeltaTime * RecruitingSentinels.Num();

		if(RecruitTimer >= TimeToRecruit)
			Recruit();
	}
}

void ARecruitingArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(ASentinelPlayerCharacter::StaticClass()))
	{
		if (!TriggerBySentinelOnly)
		{
			if(ASentinelPlayerCharacter* player = Cast<ASentinelPlayerCharacter>(OtherActor))
			{
				Player = player;
				RecruitingSentinels.Add(player);
			}
		}
	} else if (ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(OtherActor))
	{
		if(Sentinels.Contains(Sentinel)) return;

		if(const ASentinelSquad* SSquad = Sentinel->GetSquad())
			if(const ASentinelCharacter* SquadPrincipal = SSquad->GetPrincipal())
				if(SquadPrincipal->IsA(ASentinelPlayerCharacter::StaticClass())) // Sentinel is allied with player
				{
					RecruitingSentinels.Add(Sentinel);
				}
	}
}

void ARecruitingArea::OnOverlapEnd(AActor* OtherActor)
{
	if(ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(OtherActor))
	{
		RecruitingSentinels.Remove(Sentinel);
			UE_LOG(LogTemp, Log, TEXT("Stopped Recruiting, left: %d"), RecruitingSentinels.Num());
	}
}

void ARecruitingArea::OverlapTick(AActor* OtherActor, float DeltaTime)
{
	if (!TriggerBySentinelOnly && OtherActor->IsA(ASentinelPlayerCharacter::StaticClass()))
	{
		if(ASentinelPlayerCharacter* player = Cast<ASentinelPlayerCharacter>(OtherActor))
		{
			BeingRecruited = true;
			RecruitingSentinels.Add(player);

		}
	} else if (ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(OtherActor))
	{
		if(ASentinelCharacter* Principal = Sentinel->GetSquad()->GetPrincipal())
			if(ASentinelPlayerCharacter* player = Cast<ASentinelPlayerCharacter>(Principal))
			{
				BeingRecruited = true;
				Player = player;
			}
	}	
}

float ARecruitingArea::GetRecruitProgress() const
{
	return RecruitTimer / TimeToRecruit;
}

void ARecruitingArea::Recruit()
{
	for(ASentinelCharacter* Sentinel : Sentinels)
	{
		Sentinel->SetFaction(Player->GetFactionIdx());
		Sentinel->SetSquad(Player->GetSquad());
				
		const ASentinelController* SentinelController = Sentinel->GetSentinelController();
				
		SentinelController->SetPrincipal(Player);
		SentinelController->GetFollowComponent()->SetToFollow(Player);

		Sentinel->SetRecruitArea(nullptr);
	}

	for(AActor* Actor : ToRemove)
	{
		if(!Actor->IsValidLowLevel()) continue;
		Actor->Destroy();
	}


	Destroy();
}


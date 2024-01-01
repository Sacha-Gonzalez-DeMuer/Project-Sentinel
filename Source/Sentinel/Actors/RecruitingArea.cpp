// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Actors/RecruitingArea.h"

#include "Components/BoxComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/SentinelPlayerCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"

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
	
}

// Called every frame
void ARecruitingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARecruitingArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ASentinelPlayerCharacter::StaticClass()))
	{
		if(ASentinelPlayerCharacter* Player = Cast<ASentinelPlayerCharacter>(OtherActor))
		{
			for(ASentinelCharacter* Sentinel : Sentinels)
			{
				Sentinel->SetFaction(Player->GetFactionIdx());
				Sentinel->GetSentinelController()->SetPrincipal(Player);
			}

			for(AActor* Actor : ToRemove)
			{
				Actor->Destroy();
			}
		}
	}
}


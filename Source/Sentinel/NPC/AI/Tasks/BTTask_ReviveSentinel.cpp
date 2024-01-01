// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_ReviveSentinel.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/NPCBase.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTTask_ReviveSentinel::UBTTask_ReviveSentinel()
{
	NodeName = "Revive Sentinel";
}

EBTNodeResult::Type UBTTask_ReviveSentinel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCBase* OwnerNPCBase = Cast<ANPCBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!OwnerNPCBase)
	{
		UE_LOG(LogTemp, Error, TEXT("[UBTTask_ReviveSentinel::ExecuteTask] FAILED TO SETUP NPC"));
		return EBTNodeResult::Failed;
	}

	if(ASentinelCharacter* Patient = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SentinelToRevive.SelectedKeyName)))
	{
		ToRevive = Patient;
		if(ToRevive->IsOnLastStand())
		{
			ToRevive->GetHealthComponent()->Revive();
			KillNearbySentinels(OwnerNPCBase);
		}
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_ReviveSentinel::KillNearbySentinels(ANPCBase* OwnerNPCBase)
{
	const FVector NPCLocation = OwnerNPCBase->GetActorLocation();

	// Get all sentinel characters within the specified radius
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASentinelCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if(!Actor->IsValidLowLevel()) continue;
		
		ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Actor);
		if (Sentinel && Sentinel != OwnerNPCBase && !Sentinel->IsAlly(OwnerNPCBase))
		{
			const float DistanceToSentinel = FVector::DistSquared(NPCLocation, Sentinel->GetActorLocation());

			// Check if the sentinel is within the specified radius
			if (DistanceToSentinel <= DamageRadius * DamageRadius)
			{
				// Kill the sentinel     
				if(UHealthComponent* SentinelHealthComponent = Sentinel->GetHealthComponent())
					SentinelHealthComponent->TakeDamage(DamageToNearbyEnemySentinels, OwnerNPCBase);
				else
					UE_LOG(LogTemp, Error, TEXT("[UBTTask_ReviveSentinel::KillNearbySentinels] No Health Component?"));
			}
		}
	}
}

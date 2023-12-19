// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_ReviveSentinel.h"

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
	if(ASentinelCharacter* Patient = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SentinelToRevive.SelectedKeyName)))
	{
		ToRevive = Patient;
		if(ToRevive->IsOnLastStand())
		{
			ToRevive->GetHealthComponent()->Revive();
			KillNearbySentinels(OwnerComp, NodeMemory);
		}
	
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;;
}

void UBTTask_ReviveSentinel::KillNearbySentinels(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCBase* NPC = Cast<ANPCBase>(OwnerComp.GetOwner());
	const FVector NPCLocation = NPC->GetActorLocation();

	// Get all sentinel characters within the specified radius
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASentinelCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if(!Actor->IsValidLowLevel()) continue;
		
		ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Actor);
		if (Sentinel && Sentinel != NPC && !Sentinel->IsAlly(NPC))
		{
			const float DistanceToSentinel = FVector::DistSquared(NPCLocation, Sentinel->GetActorLocation());

			// Check if the sentinel is within the specified radius
			if (DistanceToSentinel <= DamageRadius * DamageRadius)
			{
				// Kill the sentinel
				Sentinel->GetHealthComponent()->TakeDamage(DamageToNearbyEnemySentinels, NPC);
			}
		}
	}
}

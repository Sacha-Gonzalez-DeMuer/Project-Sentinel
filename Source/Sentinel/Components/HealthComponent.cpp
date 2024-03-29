// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Components/HealthComponent.h"

#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	: LastStandTime(3.0f), CurrentHealth(MaxHealth), ReviveRechargeTime(1.0f)
	, ReviveCooldownTime(1.0f), ReviveCooldownTimer(0.0f), LastStandTimer(0), Character(nullptr), _IsOnLastStand(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::TakeDamage(const int Amount, ASentinelCharacter* Instigator)
{
	if (!CanTakeDamage)
		return;
	
	CurrentHealth -= Amount;
	OnTakeDamage.Broadcast(Amount);
	
	if(ASentinelController* SentinelController = Character->GetSentinelController())
		if (!Instigator->IsAlly(Character))
	{
		SentinelController->AddSeenThreat(Instigator);
	}
	
	if (CurrentHealth <= 0)
		Die();
}

void UHealthComponent::Heal(int Amount)
{
	CurrentHealth += Amount;
	
	if(CurrentHealth > MaxHealth)
	{
		Revive();
	}
}

bool UHealthComponent::GetCanTakeDamage() const
{
	return CanTakeDamage;
}

void UHealthComponent::SetCanTakeDamage(bool CanThisTakeDamage)
{
	CanTakeDamage = CanThisTakeDamage;
}

int UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

int UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthInPercent() const
{
	return static_cast<float>(CurrentHealth) / MaxHealth;
}

float UHealthComponent::GetLastStandTimerPercent() const
{
	if(LastStandTime == 0) return 0;
	
	return LastStandTimer / LastStandTime;
}

void UHealthComponent::SetParentCharacter(ASentinelCharacter* Parent)
{
	Character = Parent;
}

void UHealthComponent::SetCanBeRevived(bool _CanBeRevived)
{
	CanBeRevived = _CanBeRevived;
}

bool UHealthComponent::IsOnLastStand() const
{
	return _IsOnLastStand;
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth > 0;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::Die()
{
	UE_LOG(LogTemp, Log, TEXT("[UHealthComponent::Die] %s has died. CanBeRevived: %d, ReviveCooldownTimer: %f, !_IsOnLastStand: %d"),
	  *GetOwner()->GetName(), CanBeRevived, ReviveCooldownTimer, !_IsOnLastStand);

	
	if(CanBeRevived && ReviveCooldownTimer <= 0 && !_IsOnLastStand)
	{
		UE_LOG(LogTemp, Log, TEXT("final stand"));
		// downed state
		OnLastStand.Broadcast();
		CanBeRevived = false;
		_IsOnLastStand = true;
		LastStandTimer = LastStandTime;
		ReviveCooldownTimer = ReviveCooldownTime;
	}
	else if (ReviveCooldownTimer <= 0 && !_IsOnLastStand)
	{
		UE_LOG(LogTemp, Log, TEXT("deded x.x"));
		_IsOnLastStand = false;
		Character->OnDeath();
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Revive()
{
	UE_LOG(LogTemp, Log, TEXT("RESSURECTION"));

	_IsOnLastStand = false;
	ReviveCooldownTimer = ReviveCooldownTime;
	Character->OnRevive();
	CurrentHealth = MaxHealth;
	OnRevive.Broadcast();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(ReviveCooldownTimer > 0)
		ReviveCooldownTimer -= DeltaTime;

	if(LastStandTimer > 0)
	{
		LastStandTimer -= DeltaTime;

		if(LastStandTimer <= 0)
		{
			Die();
		}
	}
	// ...
}



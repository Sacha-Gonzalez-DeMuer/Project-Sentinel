 // Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/SquadDirectorController.h"

#include "BlackboardKeys.h"
#include "SentinelController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/Components/HealthComponent.h"

 ASquadDirectorController::ASquadDirectorController(FObjectInitializer const& ObjectInitializer)
 {
   	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
   	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
 	
 }

 void ASquadDirectorController::BeginPlay()
 {
	Super::BeginPlay();
 	if (APawn* ControlledPawn = GetPawn())
 		Squad = Cast<ASentinelSquad>(ControlledPawn);

 	
 	if (IsValid(BehaviorTree.Get()))
 	{
 		RunBehaviorTree(BehaviorTree.Get());
 		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
 	}
 }

 void ASquadDirectorController::Tick(float DeltaSeconds)
 {
	Super::Tick(DeltaSeconds);
	UE_LOG(LogTemp, Log, TEXT("directorcontrollertick"));
 	UpdatePrincipalPressure(DeltaSeconds);
 }

 void ASquadDirectorController::InitializeBlackboardKeys() const 
 {
 	if(BlackboardComponent)
 	{
 		if(Squad)
 			BlackboardComponent->SetValueAsObject(FName(BBKeys::SelfActor), Squad);
 		else
 			UE_LOG(LogTemp, Error, TEXT("[ASquadDirectorController::InitializeBlackboardKeys] Squad not linked to Blackboard."));
 	}
 }

 void ASquadDirectorController::UpdatePrincipalPressure(float DeltaSeconds)
 {
 	if(UpdatePrincipalPressureTimer > 0.0f)
 	{
 		UpdatePrincipalPressureTimer -= DeltaSeconds;

 		if(UpdatePrincipalPressureTimer <= 0.0f)
 		{
 			if(ASentinelCharacter* Principal = GetCurrentPrincipal())
 			{
 				BlackboardComponent->SetValueAsFloat(FName(BBKeys::PressureOnPrincipal), Squad->CalculatePressure(Principal));
 				UpdatePrincipalPressureTimer = UpdatePrincipalPressureInterval;
 			}

 			UpdateWeakestAgent();
 		}
 	}
 }

 void ASquadDirectorController::UpdateWeakestAgent()
 {
 	ASentinelCharacter* Weakest = Squad->GetSentinels().Top();
 	if(!Weakest) return;
 	
 	for(ASentinelCharacter* Sentinel : Squad->GetSentinels())
 	{
 		if(Weakest->GetHealthComponent()->GetHealthInPercent() > Sentinel->GetHealthComponent()->GetHealthInPercent())
 		{
 			Weakest = Sentinel;
 		}
 	}

 	if(Weakest->GetHealthComponent()->GetHealthInPercent() < .7f)
 	{
		for(ASentinelCharacter* Sentinel : Squad->GetSentinels())
		{
			if(Sentinel == Weakest) continue;

			//if(Sentinel->GetSentinelController()->GetPrincipal() == Weakest) // being protected, not weak.
			//{
			//	BlackboardComponent->SetValueAsObject(FName(BBKeys::WeakestSentinel), nullptr); 
			//}
		}
 		BlackboardComponent->SetValueAsObject(FName(BBKeys::WeakestSentinel), Weakest);
 	} else  BlackboardComponent->SetValueAsObject(FName(BBKeys::WeakestSentinel), nullptr); // not even weak

 }

 void ASquadDirectorController::SetPrincipal(ASentinelCharacter* NewPrincipal)
 {
 	if(BlackboardComponent || !NewPrincipal)
	    BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentPrincipal), NewPrincipal);
 	else UE_LOG(LogTemp, Log, TEXT("[ASquadDirectorController::SetPrincipal] No bb?"));
 }

 ASentinelCharacter* ASquadDirectorController::GetCurrentPrincipal() const
 {
 	return Cast<ASentinelCharacter>(BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));;
 }

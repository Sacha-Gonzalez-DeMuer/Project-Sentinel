 // Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/SquadDirectorController.h"

#include "BlackboardKeys.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"

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

 void ASquadDirectorController::InitializeBlackboardKeys()
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
 		UE_LOG(LogTemp, Log, TEXT("UpdatePrincipalPressureTimer %f"), UpdatePrincipalPressureTimer);

 		UpdatePrincipalPressureTimer -= DeltaSeconds;

 		if(UpdatePrincipalPressureTimer <= 0.0f)
 			if(ASentinelCharacter* Principal = GetCurrentPrincipal())
 			{
 				UE_LOG(LogTemp, Log, TEXT("Updating pressure"));

 				BlackboardComponent->SetValueAsFloat(FName(BBKeys::PressureOnPrincipal), Squad->CalculatePressure(Principal));
 				UpdatePrincipalPressureTimer = UpdatePrincipalPressureInterval;
 			}
 	}
 }

 void ASquadDirectorController::SetPrincipal(ASentinelCharacter* NewPrincipal)
 {
 	if(BlackboardComponent || !NewPrincipal)
	    BlackboardComponent->SetValueAsObject(FName(BBKeys::CurrentPrincipal), NewPrincipal);
 	else UE_LOG(LogTemp, Log, TEXT("No bb?"));
 }

 ASentinelCharacter* ASquadDirectorController::GetCurrentPrincipal() const
 {
 	return Cast<ASentinelCharacter>(BlackboardComponent->GetValueAsObject(FName(BBKeys::CurrentPrincipal)));;
 }

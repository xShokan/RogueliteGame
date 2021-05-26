// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAIController.h"

#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"

AHumanAIController::AHumanAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/AI/BT_AI.BT_AI'"));
	if (BehaviorTree.Succeeded())
	{
		AIBehaviorTree = BehaviorTree.Object;
		// UE_LOG(LogTemp, Warning, TEXT("Read BehaviorTree"));
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("Fail to read BehaviorTree"));
	}

	MyAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("MyAIPerceptionComponent"));
	SetPerceptionComponent(*MyAIPerceptionComponent);

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*Sight);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AHumanAIController::OnTargetPerceptionUpdated);
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AHumanAIController::OnPerceptionUpdated);

	DamageSense = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSense"));
	PerceptionComponent->ConfigureSense(*DamageSense);

	LineOfSightTimer = 4.0f;
}


void AHumanAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
	RunBehaviorTree(AIBehaviorTree);
}

void AHumanAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus AIStimulus)
{
	// UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionUpdated"));
	// if (Actor && Actor->ActorHasTag(FName("Player")) && AIStimulus.SensingSucceeded)
	if (Actor && Actor->ActorHasTag(FName("Player")))
	{
		// UE_LOG(LogTemp, Warning, TEXT("AI see the Player"));
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, AITimer);
		GetBlackboardComponent()->SetValueAsBool(FName("HasLineOfSight"), true);
		GetBlackboardComponent()->SetValueAsObject(FName("AIActor"), Actor);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("AI gives up chasing the Player"));
		AITimer = UKismetSystemLibrary::K2_SetTimer(this, TEXT("StartAITimer"), LineOfSightTimer, false);
	}
}

void AHumanAIController::SetDie()
{
	GetBlackboardComponent()->SetValueAsBool(FName("bDied"), true);
}

void AHumanAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("AHumanAIController::OnPerceptionUpdated"));
}

void AHumanAIController::StartAITimer()
{
	GetBlackboardComponent()->SetValueAsBool(FName("HasLineOfSight"), false);
	GetBlackboardComponent()->SetValueAsObject(FName("AIActor"), nullptr);
}

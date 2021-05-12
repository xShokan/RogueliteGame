// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "HumanAIController.generated.h"

UCLASS()
class ROUGELITEGAME_API AHumanAIController : public AAIController
{
	GENERATED_BODY()

	AHumanAIController();
	
	virtual void OnPossess(APawn* InPawn) override;

public:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* MyAIPerceptionComponent;
	
	UPROPERTY(EditAnywhere)
	class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere)
	FTimerHandle AITimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineOfSightTimer;

	UFUNCTION()
	void StartAITimer();

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor *Actor, FAIStimulus AIStimulus);
};

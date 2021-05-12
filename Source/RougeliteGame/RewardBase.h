// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "RewardBase.generated.h"

UCLASS()
class ROUGELITEGAME_API ARewardBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARewardBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* RewardMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class AFirstPersonCharacter* RewardCharacter;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* CollisionComponent;
};

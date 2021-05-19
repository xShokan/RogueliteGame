// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

UCLASS()
class ROUGELITEGAME_API ATreasureChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Open();
	
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class AFirstPersonCharacter* OverlappingCharacter;

	UPROPERTY(VisibleDefaultsOnly)
	bool bCanSpawnTreasure;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

UCLASS()
class ROUGELITEGAME_API ATrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
    virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Component check collision
	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* TrapMeshComponent;

	UPROPERTY(VisibleAnywhere)
	float Damage;

	UPROPERTY(VisibleDefaultsOnly)
	class AFirstPersonCharacter* DamageCharacter;

	UPROPERTY(VisibleDefaultsOnly)
	bool bOverlapping;
};

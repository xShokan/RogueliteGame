// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingDamage.generated.h"

UCLASS()
class ROUGELITEGAME_API AFloatingDamage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* DamageWidgetComponent;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* DamageWidget;

};

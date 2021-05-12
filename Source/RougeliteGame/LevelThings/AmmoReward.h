// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RewardBase.h"
#include "AmmoReward.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API AAmmoReward : public ARewardBase
{
	GENERATED_BODY()

	AAmmoReward();

public:
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

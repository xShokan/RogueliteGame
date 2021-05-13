// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RougeliteGameGameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FTakeDamage, float, CurrentHealth, float, MaxHealth, int32, AmmoNumInClip, int32, AmmoTotalNum, FString, WeaponName);


UCLASS()
class ROUGELITEGAME_API ARougeliteGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARougeliteGameGameModeBase();

	UPROPERTY(BlueprintAssignable)
	FTakeDamage OnTakeDamage;
};

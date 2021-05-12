// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBaseActor.h"
#include "GrenadeGunActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API AGrenadeGunActor : public AWeaponBaseActor
{
	GENERATED_BODY()

	AGrenadeGunActor();

public:
	virtual void Fire() override;
};

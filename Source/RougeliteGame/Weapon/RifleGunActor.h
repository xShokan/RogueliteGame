// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBaseActor.h"
#include "RifleGunActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API ARifleGunActor : public AWeaponBaseActor
{
	GENERATED_BODY()

	ARifleGunActor();

public:
	virtual void Fire() override;
};

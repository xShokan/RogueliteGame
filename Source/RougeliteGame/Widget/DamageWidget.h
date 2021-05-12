// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Ammo/AmmoBaseActor.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* DamageFigureLabel;
};

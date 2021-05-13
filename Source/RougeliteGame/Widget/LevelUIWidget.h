// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProgressBar.h"
#include "Blueprint/UserWidget.h"

#include "LevelUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API ULevelUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta=(BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* CurrentHealthLabel;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* MaxHealthLabel;

	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* AmmoNumInClipLabel;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* AmmoTotalNumLabel;

	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* WeaponNameLabel;

	UPROPERTY(VisibleDefaultsOnly)
	class ARougeliteGameGameModeBase* RougeliteGameGameMode;
    
protected:

	virtual void NativeConstruct() override;
	
    // virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void ChangeUI(float CurrentHealth, float MaxHealth, int32 AmmoNumInClip, int32 AmmoTotalNum, FString WeaponName);
};

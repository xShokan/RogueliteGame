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

	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* GoldLabel;

	UPROPERTY(Meta=(BindWidget))
	class UProgressBar* OtherPlayerHealthBar;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* OtherPlayerNameLabel;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* OtherPlayerCurrentHealthLabel;

	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* OtherPlayerMaxHealthLabel;

	UPROPERTY(VisibleDefaultsOnly)
	class AFirstPersonCharacter* FirstPersonCharacter;

	UPROPERTY(VisibleDefaultsOnly)
	class AFirstPersonCharacter* OtherPlayerCharacter;

	UPROPERTY()
	TArray<class AActor*> FirstPersonCharacterArray;

	class ACharacterPlayerState* PS;
	
    
protected:

	virtual void NativeConstruct() override;
	
    // virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void ChangeHealthAndAmmo(float CurrentHealth, float MaxHealth, int32 AmmoNumInClip, int32 AmmoTotalNum, FString WeaponName);

	UFUNCTION()
	void ChangeGold(int32 GoldCount);

	UFUNCTION()
	void ChangeOtherPlayerInfo(float CurrentHealth, float MaxHealth, FString PlayerName);
};

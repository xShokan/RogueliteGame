// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartGameUI.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API UStartGameUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(Meta=(BindWidget))
	class UButton* NewGameButton;

	UPROPERTY(Meta=(BindWidget))
	class UButton* QuitGameButton;

	UFUNCTION()
	void NewGameClick();

	UFUNCTION()
	void QuitGameClick();
};

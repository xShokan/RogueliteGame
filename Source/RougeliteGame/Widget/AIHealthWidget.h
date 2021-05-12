// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "AI/AICharacter.h"
#include "Blueprint/UserWidget.h"
#include "AIHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API UAIHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Meta=(BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* CurrentHealthLabel;
	
	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* MaxHealthLabel;
	
	TWeakObjectPtr<AAICharacter> OwnerCharacter;

	void SetOwnerCharacter(AAICharacter* InCharacter) {OwnerCharacter = InCharacter;};

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

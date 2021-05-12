// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHealthWidget.h"


#include "AI/AICharacter.h"
#include "ProgressBar.h"
#include "TextBlock.h"

void UAIHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerCharacter.IsValid())
		return;

	HealthBar->SetPercent(OwnerCharacter->CurrentHealth / OwnerCharacter->MaxHealth);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerCharacter->CurrentHealth));
	MaxHealthLabel->SetText(FText::AsNumber((OwnerCharacter->MaxHealth)));

}

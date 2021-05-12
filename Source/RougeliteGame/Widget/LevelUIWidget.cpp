// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUIWidget.h"

#include "TextBlock.h"
#include "Weapon/WeaponBaseActor.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"


void ULevelUIWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("ULevelUIWidget::NativeConstruct"));
}

void ULevelUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	OwnerCharacter = Cast<AFirstPersonCharacter>(Character);
	
	if (!OwnerCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter fail"));
		return;
	}

	HealthBar->SetPercent(OwnerCharacter->CurrentHealth / OwnerCharacter->MaxHealth);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerCharacter->CurrentHealth));
	MaxHealthLabel->SetText(FText::AsNumber((OwnerCharacter->MaxHealth)));

	AmmoNumInClipLabel->SetText(FText::AsNumber(OwnerCharacter->Weapon->AmmoNumInClip));
	AmmoTotalNumLabel->SetText(FText::AsNumber(OwnerCharacter->Weapon->AmmoTotalNum));
	WeaponNameLabel->SetText(FText::AsCultureInvariant(OwnerCharacter->Weapon->Name));
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUIWidget.h"



#include "FirstPersonCharacter.h"
#include "TextBlock.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"



void ULevelUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FirstPersonCharacter = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (FirstPersonCharacter)
	{
		FirstPersonCharacter->OnUIChange.BindUObject(this, &ULevelUIWidget::ChangeHealthAndAmmo);
		FirstPersonCharacter->OnGoldAdd.AddDynamic(this, &ULevelUIWidget::ChangeGold);
	}
}

/*void ULevelUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HealthBar->SetPercent(OwnerCharacter->CurrentHealth / OwnerCharacter->MaxHealth);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerCharacter->CurrentHealth));
	MaxHealthLabel->SetText(FText::AsNumber((OwnerCharacter->MaxHealth)));

	AmmoNumInClipLabel->SetText(FText::AsNumber(OwnerCharacter->Weapon->AmmoNumInClip));
	AmmoTotalNumLabel->SetText(FText::AsNumber(OwnerCharacter->Weapon->AmmoTotalNum));
	WeaponNameLabel->SetText(FText::AsCultureInvariant(OwnerCharacter->Weapon->Name));
}*/

void ULevelUIWidget::ChangeHealthAndAmmo(float CurrentHealth, float MaxHealth, int32 AmmoNumInClip, int32 AmmoTotalNum, FString WeaponName)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
	CurrentHealthLabel->SetText(FText::AsNumber(CurrentHealth));
	MaxHealthLabel->SetText(FText::AsNumber(MaxHealth));

	AmmoNumInClipLabel->SetText(FText::AsNumber(AmmoNumInClip));
	AmmoTotalNumLabel->SetText(FText::AsNumber(AmmoTotalNum));
	WeaponNameLabel->SetText(FText::AsCultureInvariant(WeaponName));
}

void ULevelUIWidget::ChangeGold(int32 GoldCount)
{
	GoldLabel->SetText(FText::AsNumber(GoldCount));
}

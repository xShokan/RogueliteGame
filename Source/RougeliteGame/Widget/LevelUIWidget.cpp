// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUIWidget.h"


#include "RougeliteGameGameModeBase.h"
#include "TextBlock.h"
#include "Weapon/WeaponBaseActor.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"


void ULevelUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RougeliteGameGameMode = Cast<ARougeliteGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (RougeliteGameGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter fail"));
		RougeliteGameGameMode->OnTakeDamage.AddDynamic(this, &ULevelUIWidget::ChangeUI);
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

void ULevelUIWidget::ChangeUI(float CurrentHealth, float MaxHealth, int32 AmmoNumInClip, int32 AmmoTotalNum, FString WeaponName)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
	CurrentHealthLabel->SetText(FText::AsNumber(CurrentHealth));
	MaxHealthLabel->SetText(FText::AsNumber(MaxHealth));

	AmmoNumInClipLabel->SetText(FText::AsNumber(AmmoNumInClip));
	AmmoTotalNumLabel->SetText(FText::AsNumber(AmmoTotalNum));
	WeaponNameLabel->SetText(FText::AsCultureInvariant(WeaponName));
}

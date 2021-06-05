// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUIWidget.h"


#include "CharacterPlayerState.h"
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
		FirstPersonCharacter->OnUIChange.AddUObject(this, &ULevelUIWidget::ChangeHealthAndAmmo);
		FirstPersonCharacter->OnGoldAdd.AddDynamic(this, &ULevelUIWidget::ChangeGold);

		PS = FirstPersonCharacter->GetPlayerState<ACharacterPlayerState>();
		if (PS)
		{
			PS->OnOtherPlayerCurrentHealth.BindUObject(this, &ULevelUIWidget::ChangeOtherPlayerInfo);
		}
	}
}

/*void ULevelUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (FirstPersonCharacter)
	{
		if (ACharacterPlayerState* PS = FirstPersonCharacter->GetPlayerState<ACharacterPlayerState>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Other Character Health %f"), PS->OtherPlayerHealth);
		}
	}
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

void ULevelUIWidget::ChangeOtherPlayerInfo(float CurrentHealth, float MaxHealth, FString PlayerName)
{
	OtherPlayerHealthBar->SetPercent(CurrentHealth / MaxHealth);
	OtherPlayerCurrentHealthLabel->SetText(FText::AsNumber(CurrentHealth));
	OtherPlayerMaxHealthLabel->SetText(FText::AsNumber(MaxHealth));
	OtherPlayerNameLabel->SetText(FText::AsCultureInvariant(PlayerName));
}

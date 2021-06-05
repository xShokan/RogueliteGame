
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CharacterPlayerState.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_ThreeParams(FOtherPlayerCurrentHealth, float, float, FString);
UCLASS()
class ROUGELITEGAME_API ACharacterPlayerState : public APlayerState
{
	GENERATED_BODY()

	ACharacterPlayerState();

	virtual void Tick(float DeltaSeconds) override;

public:

	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing=OnRep_OtherPlayerCurrentHealth)
	float OtherPlayerCurrentHealth;

	UPROPERTY(VisibleDefaultsOnly, Replicated)
	float OtherPlayerMaxHealth;

	UPROPERTY(VisibleDefaultsOnly, Replicated)
	FString OtherPlayerName;

	FOtherPlayerCurrentHealth OnOtherPlayerCurrentHealth;

	UFUNCTION(Server, Reliable)
	void ChangeOtherPlayerCurrentHealthOnSever(float Val);
	
	UFUNCTION()
	void OnRep_OtherPlayerCurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

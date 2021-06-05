// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerState.h"

#include "UnrealNetwork.h"

ACharacterPlayerState::ACharacterPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void ACharacterPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Other Health: %f"), OtherPlayerCurrentHealth);
}

void ACharacterPlayerState::ChangeOtherPlayerCurrentHealthOnSever_Implementation(float Val)
{
	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server ChangeOtherPlayerCurrentHealth"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client ChangeOtherPlayerCurrentHealth"));
	}
	OtherPlayerCurrentHealth = Val;
}

void ACharacterPlayerState::OnRep_OtherPlayerCurrentHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_OtherPlayerCurrentHealth"));
	if (OnOtherPlayerCurrentHealth.IsBound())
	{
		OnOtherPlayerCurrentHealth.Execute(OtherPlayerCurrentHealth, OtherPlayerMaxHealth, OtherPlayerName);
	}
}

void ACharacterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterPlayerState, OtherPlayerCurrentHealth);
	DOREPLIFETIME(ACharacterPlayerState, OtherPlayerMaxHealth);
	DOREPLIFETIME(ACharacterPlayerState, OtherPlayerName);
}

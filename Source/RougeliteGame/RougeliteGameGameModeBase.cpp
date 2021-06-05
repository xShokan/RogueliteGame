// Fill out your copyright notice in the Description page of Project Settings.

#include "RougeliteGameGameModeBase.h"

#include "CharacterPlayerState.h"
#include "FirstPersonCharacter.h"

#include "Widget/FPSHUD.h"
#include "GameFramework/DefaultPawn.h"

ARougeliteGameGameModeBase::ARougeliteGameGameModeBase()
{
	DefaultPawnClass = AFirstPersonCharacter::StaticClass();
	HUDClass = AFPSHUD::StaticClass();
	PlayerStateClass = ACharacterPlayerState::StaticClass();
}

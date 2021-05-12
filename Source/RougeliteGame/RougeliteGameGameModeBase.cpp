// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "RougeliteGameGameModeBase.h"


#include "Widget/FPSHUD.h"
#include "GameFramework/DefaultPawn.h"

ARougeliteGameGameModeBase::ARougeliteGameGameModeBase()
{
	DefaultPawnClass = AFirstPersonCharacter::StaticClass();
	HUDClass = AFPSHUD::StaticClass();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameUI.h"

#include "Button.h"
#include "Kismet/GameplayStatics.h"

void UStartGameUI::NativeConstruct()
{
	NewGameButton->OnClicked.AddDynamic(this, &UStartGameUI::NewGameClick);
	QuitGameButton->OnClicked.AddDynamic(this, &UStartGameUI::QuitGameClick);
}

void UStartGameUI::NewGameClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MyMap"));
}

void UStartGameUI::QuitGameClick()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ConsoleCommand("quit");
}

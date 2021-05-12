// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"


#include "ConstructorHelpers.h"
#include "Engine/Canvas.h"

AFPSHUD::AFPSHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture2DObj(TEXT("Texture2D'/Game/Other/crosshair.crosshair'"));
	if (Texture2DObj.Succeeded())
	{
		CrosshairTexture = Texture2DObj.Object;
	}
}

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

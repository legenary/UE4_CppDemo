// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "MyHUD.generated.h"

struct Message
{
	FString message;
	float time;
	FColor frontColor;
	FColor backColor;
	UTexture2D* tex;

	Message()
	{
		// Set the default time.
		time = 5.f;
		frontColor = FColor::White;
		backColor = FColor::Black;
	}

	Message(FString iMessage, float iTime, FColor iFrontColor, FColor iBackColor)
	{
		message = iMessage;
		time = iTime;
		frontColor = iFrontColor;
		backColor = iBackColor;
		tex = 0;
	}

	Message(UTexture2D* iTex, FString iMessage, float iTime, FColor iFrontColor, FColor iBackColor)
	{
		tex = iTex;
		message = iMessage;
		time = iTime;
		frontColor = iFrontColor;
		backColor = iBackColor;
	}
};
/**
 * 
 */


UCLASS()
class CPP_DEMO_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
		UFont* hudFont;

	TArray<Message> messages;
	virtual void DrawHUD() override;
	void DrawMessages();
	void addMessage(Message msg);
};

// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.generated.h"

// A message to be sent
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

struct Icon
{
	FString name;
	UTexture2D *tex;
	Icon(): name("UNKOWN NAME"), tex(0) {}
	Icon(FString &iName, UTexture2D *iTex): name(iName), tex(iTex) {}
};

// A widget to be added to HUD
struct Widget
{
	Icon icon;
	FVector2D pos, size;
	Widget(Icon iIcon): icon(iIcon), 
		pos(FVector2D(0.f, 0.f)), size(FVector2D(200.f, 200.f)) {}

	float left() { return pos.X; }
	float right() { return pos.X + size.X; }
	float top() { return pos.Y; }
	float bottom() { return pos.Y + size.Y; }
	float width() { return size.X; }
	float height() { return size.Y; }

	bool hit(FVector2D p) {
		return p.X > left() && p.X < right() && p.Y < bottom() && p.Y > top();
	}
};

UCLASS()
class CPP_DEMO_API AMyHUD : public AHUD
{
	GENERATED_BODY()

private:
	FVector2D dims;
	TArray<Message> messages;
	TArray<Widget> widgets;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
		UFont* hudFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UTexture2D* HealthBarImage;

	void addMessage(Message msg);
	void addWidget(Widget widget);

	virtual void DrawHUD() override;
	void DrawMessages();
	void DrawHealthBar();
	void DrawWidgets();

	void ClearWidgets();

	// Control
	void LeftMouseClicked();
	Widget* heldWidget;
	void MouseMoved();
};

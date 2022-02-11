// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Avatar.h"

void AMyHUD::DrawHUD() {
	Super::DrawHUD();

	dims.X = Canvas->SizeX;
	dims.Y = Canvas->SizeY;

	DrawMessages();
	DrawHealthBar();
	DrawWidgets();
}

void AMyHUD::DrawMessages() {
	// draw whatever is in HUD messages
	for (int c = messages.Num() - 1; c >= 0; c--) {
		float ow, oh, pad = 10.f;
		GetTextSize(messages[c].message, ow, oh, hudFont, 1.f);

		float messageH = oh + 2 * pad;
		float x = 0, y = c * messageH;

		if (messages[c].tex) {
			DrawTexture(messages[c].tex, x, y, messageH, messageH, 0, 0, 1, 1);
		}
		else {
			DrawRect(FLinearColor::Red, x, y, messageH, messageH);
		}
		x += messageH;

		DrawRect(messages[c].backColor, x, y, Canvas->SizeX, messageH);
		DrawText(messages[c].message, messages[c].frontColor, x + pad, y + pad, hudFont);

		messages[c].time -= GetWorld()->GetDeltaSeconds();
		// if the message's time is up, remove it
		if (messages[c].time < 0) {
			messages.RemoveAt(c);
		}
	}
}

void AMyHUD::addMessage(Message msg) {
	messages.Add(msg);
}

void AMyHUD::addWidget(Widget widget) {
	FVector2D start(200, 200), pad(12, 12);
	widget.pos = start;
	// calculate widget pos by current HUD widgets

	for (int i = 0; i < widgets.Num(); i++) {
		widget.pos.X += widget.size.X + pad.X;
		if (widget.pos.X + widget.size.X > dims.X) {
			
			widget.pos.X = 200;
			widget.pos.Y += widget.size.Y + pad.Y;
		}
	}
	widgets.Add(widget);
}

void AMyHUD::DrawHealthBar() {
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	float imgW = 451.f/1.5f, imgH = 121.f/1.5f, imgMargin = 50.f,
		barMarginY = 0.12f, barMarginX1 = 0.18f, barMarginX2 = 0.04f;
	float percHp = avatar->getHP() / avatar->getMaxHP();

	float barX = dims.X - imgMargin - imgW * (1 - barMarginX1),
		barY = dims.Y - imgMargin - imgH * (1 - barMarginY),
		barW = imgW * (1 - barMarginX1 - barMarginX2) * percHp,
		barH = imgH * (1 - barMarginY*2);
	DrawRect(FLinearColor(1 - percHp, percHp, 0, 1), barX, barY, barW, barH);

	float imgX = dims.X - imgMargin - imgW, imgY = dims.Y - imgMargin - imgH;
	if (HealthBarImage) {
		DrawTexture(HealthBarImage, imgX, imgY, imgW, imgH, 0, 0, 1, 1);
	}

	//FString percHpString = FString::SanitizeFloat(percHp);
	//DrawText(percHpString, FColor::Black, dims.X - imgWidth - imgMargin, Canvas->SizeY - imgHeight - imgMargin, hudFont);
}

void AMyHUD::DrawWidgets() {
	// draw whatever is in HUD widgets
	for (TArray<Widget>::TIterator it = widgets.CreateIterator(); it; ++it) {
		DrawTexture(it->icon.tex, it->left(), it->top(), it->width(), it->height(), 0, 0, 1, 1);
		DrawText(it->icon.name, FLinearColor::Yellow, it->left(), it->top(), hudFont, 2.f);
	}

}

void AMyHUD::ClearWidgets() {
	// unload HUD widgets
	widgets.Empty();
}

void AMyHUD::LeftMouseClicked() {
	FVector2D mouse;
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(mouse.X, mouse.Y);
	heldWidget = nullptr;
	for (TArray<Widget>::TIterator it = widgets.CreateIterator(); it; it++) {
		if (it->hit(mouse)) {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, 
					"Mouse Clicked on item...");
			}
			heldWidget = &(*it);
			return;
		}
		else {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, 
					"Mouse Clicked off item...");
			}
		}
	}
}

void AMyHUD::MouseMoved() {
	static FVector2D lastMouse;
	FVector2D thisMouse, dMouse;
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(thisMouse.X, thisMouse.Y);
	dMouse = thisMouse - lastMouse;

	float time = PController->GetInputKeyTimeDown(EKeys::LeftMouseButton);
	if (time > 0.f && heldWidget) {
			heldWidget->pos.X += dMouse.X;
			heldWidget->pos.Y += dMouse.Y;
	}
	lastMouse = thisMouse;
}
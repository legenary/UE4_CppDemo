// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

void AMyHUD::DrawHUD() {
	Super::DrawHUD();

	DrawMessages();
}

void AMyHUD::DrawMessages() {
	for (int c = messages.Num() - 1; c >= 0; c--) {
		float ow, oh, pad = 10.f;
		GetTextSize(messages[c].message, ow, oh, hudFont, 1.f);

		float messageH = oh + 2 * pad;
		float x = 0, y = c * messageH;

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
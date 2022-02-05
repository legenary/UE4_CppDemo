// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

void AMyHUD::DrawHUD() {
	Super::DrawHUD();

	DrawMessages();
	DrawHealthBar();
}

void AMyHUD::DrawMessages() {
	for (int c = messages.Num() - 1; c >= 0; c--) {
		float ow, oh, pad = 10.f;
		GetTextSize(messages[c].message, ow, oh, hudFont, 1.f);

		float messageH = oh + 2 * pad;
		float x = 0, y = c * messageH;

		if (messages[c].tex)
		{
			DrawTexture(messages[c].tex, x, y, messageH, messageH, 0, 0, 1, 1);
		}
		else
		{
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

void AMyHUD::DrawHealthBar() {
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	float barWidth = 200, barHeight = 50, barPad = 12, barMargin = 50;
	float percHp = avatar->getHP() / avatar->getMaxHP();
	DrawRect(FLinearColor(0, 0, 0, 1), Canvas->SizeX - barWidth - barPad - barMargin,
		Canvas->SizeY - barHeight - barPad - barMargin, barWidth + 2 * barPad, barHeight + 2 * barPad);
	DrawRect(FLinearColor(1 - percHp, percHp, 0, 1), Canvas->SizeX - barWidth - barMargin,
		Canvas->SizeY - barHeight - barMargin, barWidth*percHp, barHeight);
	FString percHpString = FString::SanitizeFloat(percHp);
	DrawText(percHpString, FColor::Black, Canvas->SizeX - barWidth - barMargin, Canvas->SizeY - barHeight - barMargin, hudFont);
}
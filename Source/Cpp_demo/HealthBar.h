// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"


UCLASS()
class CPP_DEMO_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	inline void SetOwner(AMonster* mon) { owner = mon; }
	
protected:
	// need to manually add progress bar in blueprint
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	AMonster* owner;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};

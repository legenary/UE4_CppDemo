// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spell.generated.h"

UCLASS()
class CPP_DEMO_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell(const class FObjectInitializer& PCIP);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Spell)
		class UBoxComponent* ProxBox;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Spell)
		class UParticleSystemComponent* Particles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
		float DamagePerSecond = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spell)
		float Duration = 3;
	
	float TimeAlive = 0;
	ACharacter* caster = nullptr;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	inline void SetCaster(ACharacter* own) { caster = own; }
	TArray<AActor*> thingsHit;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

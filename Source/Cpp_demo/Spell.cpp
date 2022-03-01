// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Monster.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Constructor
ASpell::ASpell(const class FObjectInitializer& PCIP) : Super(PCIP)
{
 	PrimaryActorTick.bCanEverTick = true;

	ProxBox = PCIP.CreateDefaultSubobject<UBoxComponent>
		(this, TEXT("ProxBox"));
	Particles = PCIP.CreateDefaultSubobject<UParticleSystemComponent>
		(this, TEXT("ParticleSystem"));

	RootComponent = Particles;
	ProxBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProxBox->GetOverlappingActors(thingsHit);
	// then, damage each actor
	for (int c = 0; c < thingsHit.Num(); c++) {
		AMonster* monster = Cast<AMonster>(thingsHit[c]);
		if (monster && ProxBox->IsOverlappingComponent(monster->GetCapsuleComponent())) {
			monster->TakeDamage(DamagePerSecond * DeltaTime, FDamageEvent(), 0, this);
		}
	}

	TimeAlive += DeltaTime;

	if (TimeAlive > Duration) {
		Destroy();
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Monster.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

ABullet::ABullet(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	ProxSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProxSphere"));

	RootComponent = ProxSphere;
	Mesh->SetupAttachment(RootComponent);

	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::Prox);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	existTime += DeltaTime;
	if (existTime > dieTime) {
		Destroy();
	}
}

void ABullet::Prox_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {

	if (OtherComp != OtherActor->GetRootComponent()) {	 // if not hit root 
		return;
	}

	if (OtherActor == shooter) {	 // if hit shooter
		return;
	}
	
	OtherActor->TakeDamage(Damage + damageFromHolder, FDamageEvent(), nullptr, this);
	Destroy();
}
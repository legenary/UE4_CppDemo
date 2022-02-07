// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Monster.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	Damage = 1;
	swinging = false;
	holder = nullptr;

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;

	ProxBox = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("ProxBox"));
	ProxBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::Prox);
	ProxBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeapon::Prox_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	
	if (!swinging ||								// if not swinging
		Cast<AMonster>(OtherActor) == holder ||		// if weapon holder
		OtherComp != OtherActor->GetRootComponent()	// if didn't hit root component
		) {
		return;
	}

	if (!thingsHit.Contains(OtherActor)) {
		thingsHit.Add(OtherActor);
		//OtherActor->TakeDamage(Damage + holder->BaseAttackDamage, FDamageEvent(), nullptr, this);
	}

}

void AMeleeWeapon::Swing() {
	thingsHit.Empty();
	swinging = true;
}

void AMeleeWeapon::Reset() {
	thingsHit.Empty();
	swinging = false;
}


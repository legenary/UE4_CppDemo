// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Monster.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Constructor
AMeleeWeapon::AMeleeWeapon(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	//PrimaryActorTick.bCanEverTick = true;

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

// for hitting on actor in one swing
void AMeleeWeapon::Prox_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {

	
	if (!swinging ) { // if not swinging
		//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "not swinging");
		return;
	}

	if (OtherComp != OtherActor->GetRootComponent()) { // if didn't hit root component
		//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "not root");
		return;
	}

	if (OtherActor == holder) { // if weapon holder
		//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "touch self");
		return; 
	}
	
	//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "pass");
	if (!thingsHit.Contains(OtherActor)) {
		thingsHit.Add(OtherActor);
		OtherActor->TakeDamage(Damage + damageFromHolder, FDamageEvent(), nullptr, this);
	}

}

void AMeleeWeapon::ResetHitList() {
	thingsHit.Empty();
}


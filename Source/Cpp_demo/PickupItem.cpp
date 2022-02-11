// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Avatar.h"
#include "MyHUD.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Constructor
APickupItem::APickupItem(const class FObjectInitializer& PCIP) : Super(PCIP)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Name = "UNKOWN ITEM";
	Quantity = 0;

	ProxSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProxSphere"));
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));

	RootComponent = Mesh;
	ProxSphere->SetupAttachment(RootComponent);
	Mesh->SetSimulatePhysics(true);

	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::Prox);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::Prox_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	// if otherActor is not AAvatar, simply return
	if (Cast<AAvatar>(OtherActor) == nullptr) {
		return;
	}

	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	avatar->PickUp(this);

	APlayerController *PController = GetWorld()->GetFirstPlayerController();
	AMyHUD *hud = Cast<AMyHUD>(PController->GetHUD());
	FString msg = "Picked up " + FString::FromInt(Quantity) + FString(" ") + Name + 
		" (Now: " + FString::FromInt(avatar->backpack[Name].quantity) + ")";
	hud->addMessage(Message(Icon, msg, 5.f, FColor::White, FColor::Transparent));

	Destroy();
}


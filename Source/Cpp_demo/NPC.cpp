// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "MyHUD.h"
#include "Avatar.h"

#include "Components/SphereComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// override constructor
ANPC::ANPC(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	ProxSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProxSphere"));

	// Attach the ProxSphere to the root component
	ProxSphere->SetupAttachment(RootComponent);
	ProxSphere->SetSphereRadius(100.f);

	// Code to make ANPC::Prox() run when this proximity sphere overlaps another actor.
	FName TestName = FName(TEXT("ThisIsMyTestFName"));
	ProxSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::Prox);
	NpcMessage = "Hi, I'm Owen";//default message, can be edited in blueprints
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Prox_Implementation(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	// if otherActor is not AAvatar, simply return
	if (Cast<AAvatar>(OtherActor) == nullptr) {
		return;
	}
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (PController) {
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		FString msg = FString("My name is ") + Name + FString(". ") + NpcMessage;
		hud->addMessage(Message(Face, msg, 5.f, FColor::White, FColor::Transparent));

		// decrease HP
		AAvatar *avatar = Cast<AAvatar>(OtherActor);
		avatar->decreaseHP(5.f);
	}
}

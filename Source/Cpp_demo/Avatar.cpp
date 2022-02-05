// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "Avatar.h"

// Sets default values
AAvatar::AAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 80;
	maxHP = 100;
}

// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AAvatar::ToggleInventory);

	InputComponent->BindAxis("Forward", this, &AAvatar::MoveForward);
	InputComponent->BindAxis("Back", this, &AAvatar::MoveBack);
	InputComponent->BindAxis("Left", this, &AAvatar::MoveLeft);
	InputComponent->BindAxis("Right", this, &AAvatar::MoveRight);
	InputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	InputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);

}

void AAvatar::MoveForward(float amount) {
	if (Controller && amount) {
		FVector fwd = GetActorForwardVector();
		AddMovementInput(fwd, amount);
	}
}
void AAvatar::MoveBack(float amount) {
	if (Controller && amount) {
		FVector back = -GetActorForwardVector();
		AddMovementInput(back, amount);
	}
}
void AAvatar::MoveLeft(float amount) {
	if (Controller && amount) {
		FVector left = -GetActorRightVector();
		AddMovementInput(left, amount);
	}
}
void AAvatar::MoveRight(float amount) {
	if (Controller && amount) {
		FVector right = GetActorRightVector();
		AddMovementInput(right, amount);
	}
}
void AAvatar::Yaw(float amount) {
	AddControllerYawInput(100.f * amount * GetWorld()->GetDeltaSeconds());
}
void AAvatar::Pitch(float amount) {
	AddControllerPitchInput(-100.f * amount * GetWorld()->GetDeltaSeconds());
}



void AAvatar::ToggleInventory() {
	if (GEngine) {
		FString str = "Showing Inventory...";
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, str);
	}
}
void AAvatar::Pick(APickupItem *item) {
	if (backpack.Find(item->Name)) {
		backpack[item->Name] += item->Quantity;
	}
	else {
		backpack.Add(item->Name, item->Quantity);
		icons.Add(item->Name, item->Icon);
	}
}



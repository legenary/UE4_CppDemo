// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#
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
	if (inventoryShowing) { return; }
	if (Controller && amount) {
		FVector fwd = GetActorForwardVector();
		AddMovementInput(fwd, amount);
	}
}
void AAvatar::MoveBack(float amount) {
	if (inventoryShowing) { return; }
	if (Controller && amount) {
		FVector back = -GetActorForwardVector();
		AddMovementInput(back, amount);
	}
}
void AAvatar::MoveLeft(float amount) {
	if (inventoryShowing) { return; }
	if (Controller && amount) {
		FVector left = -GetActorRightVector();
		AddMovementInput(left, amount);
	}
}
void AAvatar::MoveRight(float amount) {
	if (inventoryShowing) { return; }
	if (Controller && amount) {
		FVector right = GetActorRightVector();
		AddMovementInput(right, amount);
	}
}
void AAvatar::Yaw(float amount) {
	if (inventoryShowing) { return; }
	AddControllerYawInput(100.f * amount * GetWorld()->GetDeltaSeconds());
}
void AAvatar::Pitch(float amount) {
	if (inventoryShowing) { return; }
	AddControllerPitchInput(-100.f * amount * GetWorld()->GetDeltaSeconds());
}



void AAvatar::ToggleInventory() {
	if (GEngine) {
		APlayerController *PController = GetWorld()->GetFirstPlayerController();
		AMyHUD *hud = Cast<AMyHUD>(PController->GetHUD());

		if (inventoryShowing) {
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Closing Inventory...");
			hud->ClearWidgets();
			inventoryShowing = false;
			PController->bShowMouseCursor = false;
		}
		else {
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Showing Inventory...");
			inventoryShowing = true;
			PController->bShowMouseCursor = true;
			for (TMap<FString, int>::TIterator it = backpack.CreateIterator(); it; ++it) {
				FString fs = it->Key + " x " + FString::FromInt(it->Value);
				UTexture2D* tex = 0;
				if (icons.Find(it->Key)) {
					tex = icons[it->Key];
				}
				hud->addWidget(Widget(Icon(fs, tex)));
			}
		}
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



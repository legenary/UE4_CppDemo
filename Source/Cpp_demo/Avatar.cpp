// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "MeleeWeapon.h"
#include "Avatar.h"
#include "Spell.h"

// Sets default values
AAvatar::AAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 80;
	maxHP = 100;
}

void AAvatar::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BPMeleeWeapon) {
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());
		if (MeleeWeapon) {
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("Muzzle_01");
			socket->AttachActor(Cast<AActor>(MeleeWeapon), this->GetMesh());
			MeleeWeapon->holder = this;
			MeleeWeapon->damageFromHolder = this->BaseMeleeDamage;
		}
	}
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
	InputComponent->BindAction("MouseClickedMLB", IE_Pressed, this, &AAvatar::LeftMouseClicked);
	InputComponent->BindAction("MouseClickedMRB", IE_Pressed, this, &AAvatar::RightMouseClicked);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AAvatar::StartJump);

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
	//if (inventoryShowing) { return; }
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
	if (inventoryShowing) { 
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		hud->MouseMoved();
	}
	else {
		AddControllerYawInput(100.f * amount * GetWorld()->GetDeltaSeconds());
	}
}
void AAvatar::Pitch(float amount) {
	if (inventoryShowing) {
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
		hud->MouseMoved();
	}
	else {
		AddControllerPitchInput(-100.f * amount * GetWorld()->GetDeltaSeconds());
	}
}

void AAvatar::LeftMouseClicked() {
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());

	// if inventory is open, call MouseClicked from HUD
	if (inventoryShowing) {
		hud->LeftMouseClicked();
	}
	else {
		Melee();
	}
}

void AAvatar::RightMouseClicked() {
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	AMyHUD* hud = Cast<AMyHUD>(PController->GetHUD());
	if (inventoryShowing) {
		return;
	}
	else {
		CastAnimation();
	}
}



void AAvatar::ToggleInventory() {
	if (GEngine) {
		APlayerController *PController = GetWorld()->GetFirstPlayerController();
		AMyHUD *hud = Cast<AMyHUD>(PController->GetHUD());

		if (inventoryShowing) {
			//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Closing Inventory...");
			// unload HUD widgets
			hud->ClearWidgets();
			inventoryShowing = false;
			PController->bShowMouseCursor = false;
		}
		else {
			//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Showing Inventory...");
			inventoryShowing = true;
			PController->bShowMouseCursor = true;
			for (TMap<FString, Properties>::TIterator it = backpack.CreateIterator(); it; ++it) {
				FString fs = it->Key + " x " + FString::FromInt(it->Value.quantity);

				// load backpack items to HUD widgets
				hud->addWidget(Widget(Icon(fs, it->Value.icon)));
			}
		}
	}
}
void AAvatar::Pick(APickupItem *item) {
	if (backpack.Find(item->Name)) {
		backpack[item->Name].quantity += item->Quantity;
	}
	else {
		backpack.Add(item->Name, Properties(item->Quantity, item->Icon));
	}
}

void AAvatar::finishedSwinging() {
	nMelee--;
	if (MeleeWeapon) { 
		MeleeWeapon->swinging = false;
		MeleeWeapon->ResetHitList();
	}
}

void AAvatar::StartJump() {
	Jumping = true;
}

void AAvatar::finishedJumping() {
	Jumping = false;
}

float AAvatar::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	HP -= Damage;

	// add some knockback that gets applied over a few frames
	//Knockback = GetActorLocation() - DamageCauser->GetActorLocation();
	//Knockback.Normalize();
	//Knockback *= Damage * 500;

	// if he goes below 0 hp, he will resurrect
	if (HP <= 0)
	{
		HP = maxHP; //resurrect
	}
	return Damage;
}

void AAvatar::Melee() {
	// can only increase attack number when no attack is in excution
	if (!nMelee) {
		nMelee++;
		if (MeleeWeapon) {
			MeleeWeapon->swinging = true;
			MeleeWeapon->ResetHitList();
		}
	}
}

void AAvatar::CastAnimation() {
	if (backpack.Find("Spell")) {
		backpack["Spell"].quantity -= 1;
		if (!nCast) {
			nCast++;
		}
		if (backpack["Spell"].quantity <= 0) {
			// add code to make pickup item disappear
			// ...
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "No spell left...");
		return;
	}
	// invoke animation to cast
}

void AAvatar::CastSpell() {
	// add code to spawn particles
	// ...
	FVector fwd = GetActorForwardVector();
	FVector nozzle = GetMesh()->GetBoneLocation("hand_r");
	nozzle += fwd * 10;

	ASpell* spell = GetWorld()->SpawnActor<ASpell>
		(BPSpell, nozzle, RootComponent->GetComponentRotation());
	if (spell) {
		spell->SetCaster(this);
	}
	else {
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "no spell spawned.");
	}
}

void AAvatar::finishedCasting() {
	nCast--;
}
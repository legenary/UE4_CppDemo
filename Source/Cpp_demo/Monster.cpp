// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Avatar.h"
#include "MeleeWeapon.h"
#include "Bullet.h"
#include "HealthBar.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/WidgetComponent.h"


AMonster::AMonster(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	// sight sphere
	SightSphere = PCIP.CreateDefaultSubobject<USphereComponent>
		(this, TEXT("Sight Sphere"));
	SightSphere->SetupAttachment(RootComponent);

	// melee range
	MeleeRangeSphere = PCIP.CreateDefaultSubobject<USphereComponent>
		(this, TEXT("Melee Range Sphere"));
	MeleeRangeSphere->SetupAttachment(RootComponent);

	// shooting range
	ShootingRangeSphere = PCIP.CreateDefaultSubobject<USphereComponent>
		(this, TEXT("Shooting Range Sphere"));
	ShootingRangeSphere->SetupAttachment(RootComponent);

	// health bar widget component
	HealthWidgetComp = PCIP.CreateDefaultSubobject<UWidgetComponent>
		(this, TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent,
		FAttachmentTransformRules::KeepRelativeTransform);
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BPMeleeWeapon) {
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());
		// in this script, should always check MeleeWeapon existence
		if (MeleeWeapon) {
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("Muzzle_01");
			socket->AttachActor(Cast<AActor>(MeleeWeapon), this->GetMesh());
			MeleeWeapon->holder = this;
			MeleeWeapon->damageFromHolder = this->BaseMeleeDamage;
		}
	}
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	// initialize heal bar widget
	HealthBar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	if (!HealthBar) { return; }
	HealthBar->SetOwner(this);
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// shit happens when you die, or even get hit
	if (Die || hitReact) {
		return;
	}

	AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar) { return; }

	FVector toPlayer = avatar->GetActorLocation() - this->GetActorLocation();
	float distToPlayer = toPlayer.Size();
	toPlayer.Normalize();


	if (!isInSightSphere(distToPlayer)) {
		HealthBar->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// when player in sight, facing player always
	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0;
	RootComponent->SetWorldRotation(toPlayerRotation);

	// shooting mode
	if (!MeleeWeapon && BPBullet) {
		if (!isInShootingRangeSphere(distToPlayer)) {
			HealthBar->SetVisibility(ESlateVisibility::Hidden);
			// if outside shooting range, move to player
			AddMovementInput(toPlayer, Speed*DeltaTime);
		}
		else if (isInMeleeRangeSphere(distToPlayer)) {
			HealthBar->SetVisibility(ESlateVisibility::Visible);
			// if inside melee range, move away from player (in slower speed)
			AddMovementInput(-toPlayer, Speed/2*DeltaTime);
		}
		else {
			HealthBar->SetVisibility(ESlateVisibility::Visible);
			// atack
			Attack();
			TimeSinceLastStrike += DeltaTime;
			if (TimeSinceLastStrike > StrikeTimeout) {
				TimeSinceLastStrike = 0;
			}
		}
	}
	// melee mode
	else {
		if (!isInMeleeRangeSphere(distToPlayer)) {
			HealthBar->SetVisibility(ESlateVisibility::Hidden);
			// if outside melee range, move to player
			AddMovementInput(toPlayer, Speed*DeltaTime);
			if (MeleeWeapon) {
				MeleeWeapon->swinging = false;
				MeleeWeapon->ResetHitList();
			}
			TimeSinceLastStrike = 0;
		}
		else {
			HealthBar->SetVisibility(ESlateVisibility::Visible);
			// if inside melee range, attack
			Attack();
			TimeSinceLastStrike += DeltaTime;
			if (TimeSinceLastStrike > StrikeTimeout) {
				TimeSinceLastStrike = 0;
			}
		}
	}
	
}

// combat
void AMonster::Attack() {
	// not yet
	if (TimeSinceLastStrike) {
		return;
	}
	
	// if have melee weapon
	if (MeleeWeapon) {
		if (!nMelee) {
			nMelee++;	// give the monster an attack instruct
			if (MeleeWeapon) {
				MeleeWeapon->swinging = true;
				MeleeWeapon->ResetHitList();
			}
		}
	}
	// if no weapon equipped, spawn bullet
	else if (BPBullet) {
		if (!nShoot) {
			nShoot++;	// give the monster a shooting instruct
		}
	}
}

float AMonster::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser) {

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	MonsterHP -= Damage;

	// if he goes below 0 hp, he will resurrect
	if (MonsterHP <= 0)
	{
		Die = true;
		return Damage;
	}

	// if didn't die, perform hit reaction
	hitReact = true;
	return Damage;
}


void AMonster::DestroyAll() {
	if (MeleeWeapon) {
		MeleeWeapon->Destroy();
	}
	Destroy();
}

// AnimNotify functions
void AMonster::FireBullet() {
	// fire bullet
	FVector fwd = GetActorForwardVector();
	FVector nozzle = GetMesh()->GetBoneLocation("hand_r");
	nozzle += fwd * 10;

	AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar) { return; }

	FVector toOpponent = avatar->GetActorLocation() - nozzle;
	toOpponent.Normalize();
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>
		(BPBullet, nozzle, RootComponent->GetComponentRotation());
	if (bullet) {
		bullet->SetShooter(this);
		bullet->damageFromHolder = this->BaseMeleeDamage;
		bullet->ProxSphere->AddImpulse(toOpponent * BulletLaunchImpulse);
	}
	else {
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "no bullet spawned.");
	}
}

void AMonster::FinishedSwinging() {
	nMelee--;	// attack instruct executed
	if (MeleeWeapon) {
		MeleeWeapon->swinging = false;
		MeleeWeapon->ResetHitList();
	}
}

void AMonster::FinishedShooting() {
	nShoot--;	// attack instruct executed
}

bool AMonster::isInSightSphere(float d) {
	return d < SightSphere->GetScaledSphereRadius();
}
bool AMonster::isInMeleeRangeSphere(float d) {
	return d < MeleeRangeSphere->GetScaledSphereRadius();
}
bool AMonster::isInShootingRangeSphere(float d) {
	return d < ShootingRangeSphere->GetScaledSphereRadius();
}
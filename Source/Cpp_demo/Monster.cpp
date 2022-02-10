// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

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
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// shit happens when you die
	if (Die) {
		return;
	}

	AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar) { return; }

	FVector toPlayer = avatar->GetActorLocation() - this->GetActorLocation();
	float distToPlayer = toPlayer.Size();
	toPlayer.Normalize();


	if (!isInSightSphere(distToPlayer)) {
		return;
	}

	// start facing player
	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0;
	RootComponent->SetWorldRotation(toPlayerRotation);

	// shooting mode
	if (!MeleeWeapon && BPBullet) {
		if (!isInShootingRangeSphere(distToPlayer)) {
			// if outside shooting range, move to player
			AddMovementInput(toPlayer, Speed*DeltaTime);
		}
		else if (isInMeleeRangeSphere(distToPlayer)) {
			// if inside melee range, move away from player (in slower speed)
			AddMovementInput(-toPlayer, Speed/2*DeltaTime);
		}
		else {
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
			// if outside melee range, move to player
			AddMovementInput(toPlayer, Speed*DeltaTime);
			if (MeleeWeapon) {
				MeleeWeapon->swinging = false;
				MeleeWeapon->ResetHitList();
			}
			TimeSinceLastStrike = 0;
		}
		else {
			// if inside melee range, attack
			Attack();
			TimeSinceLastStrike += DeltaTime;
			if (TimeSinceLastStrike > StrikeTimeout) {
				TimeSinceLastStrike = 0;
			}
		}
	}

	
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::finishedSwinging() {
	nMelee--;	// attack instruct executed
	if (MeleeWeapon) {
		MeleeWeapon->swinging = false;
		MeleeWeapon->ResetHitList();
	}
}

void AMonster::finishedShooting() {
	nShoot--;	// attack instruct executed
}

void AMonster::fireBullet() {
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
		bullet->shooter = this;
		bullet->damageFromHolder = this->BaseMeleeDamage;
		bullet->ProxSphere->AddImpulse(toOpponent * BulletLaunchImpulse);
	}
	else {
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "no bullet spawned.");
	}
}

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
	}
	return Damage;
}

void AMonster::DestroyAll() {
	if (MeleeWeapon) {
		MeleeWeapon->Destroy();
	}
	Destroy();
}
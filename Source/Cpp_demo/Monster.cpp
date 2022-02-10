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
			MeleeWeapon->DamageFromHolder = this->BaseAttackDamage;
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

	AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!avatar) { return; }

	FVector toPlayer = avatar->GetActorLocation() - this->GetActorLocation();
	float distToPlayer = toPlayer.Size();
	toPlayer.Normalize();


	if (!isInSightSphere(distToPlayer)) {
		return;
	}

	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0;
	RootComponent->SetWorldRotation(toPlayerRotation);

	if (!isInMeleeRangeSphere(distToPlayer)) {
		AddMovementInput(toPlayer, Speed*DeltaTime);

		if (MeleeWeapon) { 
			MeleeWeapon->swinging = false;
			MeleeWeapon->ResetHitList();
		}
		TimeSinceLastStrike = 0;
	}
	else {	// attack mode
		if (!TimeSinceLastStrike) {
			Attack(avatar);
		}
		TimeSinceLastStrike += DeltaTime;
		if (TimeSinceLastStrike > AttackTimeout) {
			TimeSinceLastStrike = 0;
		}
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::finishedSwinging() {
	nAttack--;	// attack instruct executed
	if (MeleeWeapon) {
		MeleeWeapon->swinging = false;
		MeleeWeapon->ResetHitList();
	}
}

void AMonster::Attack(AActor* thing) {
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "attacking.");
	// if have melee weapon
	if (MeleeWeapon) {
		if (!nAttack) {
			nAttack++;	// give the monster an attack instruct
			if (MeleeWeapon) {
				MeleeWeapon->swinging = true;
				MeleeWeapon->ResetHitList();
			}
		}
	}
	// if no weapon equipped, spawn bullet
	else if (BPBullet) {
		FVector fwd = GetActorForwardVector();
		FVector nozzle = GetMesh()->GetBoneLocation("hand_r");
		nozzle += fwd * 155;
		FVector toOpponent = thing->GetActorLocation() - nozzle;
		toOpponent.Normalize();
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>
			(BPBullet, nozzle, RootComponent->GetComponentRotation());
		if (bullet) {
			bullet->firer = this;
			bullet->ProxSphere->AddImpulse(fwd * BulletLaunchImpulse);
		}
		else {
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, "no bullet spawned.");
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
		Die();
	}
	return Damage;
}

void AMonster::Die() {
	if (MeleeWeapon) {
		MeleeWeapon->Destroy();
	}
	Destroy();
}
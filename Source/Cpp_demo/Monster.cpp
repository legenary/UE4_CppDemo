// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

AMonster::AMonster(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	SightSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sight Sphere"));
	SightSphere->SetupAttachment(RootComponent);

	AttackRangeSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Attack Range Sphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);

	//movingToPlayer = true;
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BPMeleeWeapon) {
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());
		if (MeleeWeapon) {
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("Muzzle_01");
			socket->AttachActor(Cast<AActor>(MeleeWeapon), this->GetMesh());
			MeleeWeapon->holder = this;
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

	if (!isInAttackRangeSphere(distToPlayer)) {
		AddMovementInput(toPlayer, Speed*DeltaTime);
		MeleeWeapon->swinging = false;
		TimeSinceLastStrike = 0;
	}
	else {	// attack mode
		if (!TimeSinceLastStrike) {
			Attack(avatar);
			if (!nAttack) {
				nAttack++;	// give the monster an attack instruct
			}
			MeleeWeapon->swinging = true;
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::FromInt(nAttack));
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
}

void AMonster::Attack(AActor* thing) {

}

void AMonster::SwordReset() {
	// reset weapon
	if (MeleeWeapon) {
		MeleeWeapon->Reset();
	}
}

float AMonster::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	MonsterHP -= Damage;

	// if he goes below 0 hp, he will resurrect
	if (MonsterHP <= 0)
	{
		Destroy();
	}
	return Damage;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

AMonster::AMonster(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	Speed = 25;
	MonsterHP = 20;
	MonsterExp = 0;
	BPLoot = nullptr;
	BaseAttackDamage = 1;
	AttackTimeout = 1.5f;
	TimeSinceLastStrike = 0;
	movingToPlayer = true;
	Attacking = false;

	SightSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sight Sphere"));
	SightSphere->SetupAttachment(RootComponent);

	AttackRangeSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Attack Range Sphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::Prox);
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BPMeleeWeapon) {
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon, FVector(), FRotator());
		if (MeleeWeapon) {
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("Muzzle_01");
			socket->AttachActor(Cast<AActor>(MeleeWeapon), this->GetMesh());
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
		//movingToPlayer = false;
		
		return;
	}

	FRotator toPlayerRotation = toPlayer.Rotation();
	toPlayerRotation.Pitch = 0;
	RootComponent->SetWorldRotation(toPlayerRotation);

	

	if (!isInAttackRangeSphere(distToPlayer)) {
		AddMovementInput(toPlayer, Speed*DeltaTime);
		Attacking = false;
	}
	else {
		Attacking = true;
	}

	

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMonster::Prox_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	//
	if (Cast<AAvatar>(OtherActor)) {
		movingToPlayer = !movingToPlayer;
	}
}

void AMonster::SwordSwung() {
	if (GEngine) {
		FString msg = "Swung";
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, msg);
	}
	if (MeleeWeapon) {
		MeleeWeapon->Swing();
		
	}
}
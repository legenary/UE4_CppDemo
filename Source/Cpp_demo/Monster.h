// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Avatar.h"
#include "MeleeWeapon.h"
#include "Bullet.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Monster.generated.h"

UCLASS()
class CPP_DEMO_API AMonster : public ACharacter
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float Speed = 70.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float MonsterHP = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		int32 MonsterExp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPLoot = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float StrikeTimeout = 2.f;
	UPROPERTY(BlueprintReadOnly, Category = MonsterProperties)
		float TimeSinceLastStrike = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPMeleeWeapon;
	AMeleeWeapon* MeleeWeapon;
	UPROPERTY(BlueprintReadOnly, Category = MonsterProperties)
		int32 nMelee = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BaseMeleeDamage = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPBullet;
	ABullet* Bullet;
	UPROPERTY(BlueprintReadOnly, Category = MonsterProperties)
		int32 nShoot = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BulletLaunchImpulse = 700.f;
	UFUNCTION(BlueprintCallable, Category = Collision)
		void fireBullet();
	UFUNCTION(BlueprintCallable, Category = Collision)
		void finishedShooting();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* SightSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* MeleeRangeSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* ShootingRangeSphere;
	UFUNCTION(BlueprintCallable, Category = Collision)
		void finishedSwinging();

	UPROPERTY(BlueprintReadOnly, Category = MonsterProperties)
		bool Die = false;
	UFUNCTION(BlueprintCallable, Category = Collision)
		void DestroyAll();

	// Sets default values for this character's properties
	AMonster();
	AMonster(const class FObjectInitializer& PCIP);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Melee weapon intializer
	virtual void PostInitializeComponents() override;

	// Monster AI
	inline bool isInSightSphere(float d) {
		return d < SightSphere->GetScaledSphereRadius();
	}
	inline bool isInMeleeRangeSphere(float d) {
		return d < MeleeRangeSphere->GetScaledSphereRadius();
	}
	inline bool isInShootingRangeSphere(float d) {
		return d < ShootingRangeSphere->GetScaledSphereRadius();
	}
	void Attack();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
};

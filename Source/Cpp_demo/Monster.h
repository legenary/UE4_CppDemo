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
		float Speed = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float MonsterHP = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		int32 MonsterExp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BaseAttackDamage = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float AttackTimeout = 2.5.f;
	UPROPERTY(BlueprintReadOnly, Category = MonsterProperties)
		int32 nAttack = 0;
	UPROPERTY(BlueprintReadOnly, Category = MonsterProperties)
		float TimeSinceLastStrike = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPLoot = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPMeleeWeapon;
	AMeleeWeapon* MeleeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPBullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BulletLaunchImpulse;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* SightSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* AttackRangeSphere;
	UFUNCTION(BlueprintCallable, Category = Collision)
		void SwordReset();
	UFUNCTION(BlueprintCallable, Category = Collision)
		void finishedSwinging();


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
	inline bool isInAttackRangeSphere(float d) {
		return d < AttackRangeSphere->GetScaledSphereRadius();
	}
	void Attack(AActor* thing);
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
};

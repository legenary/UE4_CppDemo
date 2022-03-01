// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "GameFramework/Pawn.h"
#include "Monster.generated.h"

class AMeleeWeapon;
class ABullet;
class UHealthBar;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class CPP_DEMO_API AMonster : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMonster(const class FObjectInitializer& PCIP);
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float Speed = 70.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float MonsterHP = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float MonsterMaxHP = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		int32 MonsterExp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPLoot = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float StrikeTimeout = 2.f;
	UPROPERTY(BlueprintReadOnly)
		float TimeSinceLastStrike = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPMeleeWeapon;
	AMeleeWeapon* MeleeWeapon;
	UPROPERTY(BlueprintReadOnly)
		int32 nMelee = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BaseMeleeDamage = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPBullet;
	ABullet* Bullet;
	UPROPERTY(BlueprintReadOnly)
		int32 nShoot = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BulletLaunchImpulse = 700.f;
	UFUNCTION(BlueprintCallable)
		void FireBullet();
	UFUNCTION(BlueprintCallable)
		void FinishedShooting();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class UPawnSensingComponent* PawnSensor;
	//UFUNCTION(BlueprintCallable)
	//	void OnSeePawn(APawn *OtherPawn);


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USphereComponent* SightSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USphereComponent* MeleeRangeSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USphereComponent* ShootingRangeSphere;
	UFUNCTION(BlueprintCallable)
		void FinishedSwinging();

	UPROPERTY(BlueprintReadWrite)
		bool hitReact = false;

	UPROPERTY(BlueprintReadOnly)
		bool Die = false;
	UFUNCTION(BlueprintCallable)
		void DestroyAll();

	UPROPERTY(VisibleAnywhere)
		UWidgetComponent* HealthWidgetComp;
	UHealthBar* HealthBar;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Combat
	bool isInSightSphere(float d);
	bool isInMeleeRangeSphere(float d);
	bool isInShootingRangeSphere(float d);
	void Attack();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
};

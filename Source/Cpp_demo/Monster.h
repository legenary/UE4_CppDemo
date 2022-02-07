// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Avatar.h"
#include "MeleeWeapon.h"

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
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float MonsterHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		int32 MonsterExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float BaseAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		float AttackTimeout;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MonsterProperties)
		float TimeSinceLastStrike;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPLoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterProperties)
		UClass* BPMeleeWeapon;
	AMeleeWeapon* MeleeWeapon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* SightSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MonsterProperties)
		USphereComponent* AttackRangeSphere;
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void Prox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MonsterProperties)
		bool Attacking;
	UFUNCTION(BlueprintCallable, Category = Collision)
		void SwordSwung();


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
	bool movingToPlayer;		// if in SightSphere and outside AttackRangeSphere,
								// set this to true
	inline bool isInSightSphere(float d) {
		return d < SightSphere->GetScaledSphereRadius();
	}
	inline bool isInAttackRangeSphere(float d) {
		return d < AttackRangeSphere->GetScaledSphereRadius();
	}
	
};

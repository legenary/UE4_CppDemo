// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class AMonster;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class CPP_DEMO_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet(const class FObjectInitializer& PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		float Damage = 10;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bullet)
		UStaticMeshComponent* Mesh;
	ACharacter* shooter = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bullet)
		USphereComponent* ProxSphere;
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void Prox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	inline void SetShooter(ACharacter* own) { shooter = own; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float existTime = 0.f;
	float dieTime = 3.f;
	float damageFromHolder = 0.f;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Bullet.generated.h"

class AMonster;

UCLASS()
class CPP_DEMO_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet(const class FObjectInitializer& PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		float Damage = 10;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bullet)
		UStaticMeshComponent* Mesh;
	ACharacter* shooter;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bullet)
		USphereComponent* ProxSphere;
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void Prox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float existTime = 0.f;
	float dieTime = 3.f;
	float damageFromHolder = 0.f;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyHUD.h"
#include "Avatar.h"

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

UCLASS()
class CPP_DEMO_API ANPC : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ANPC();
	ANPC(const class FObjectInitializer& PCIP);
	// Bind properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		USphereComponent* ProxSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPCMessage)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPCMessage)
		FString NpcMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPCMessage)
		UTexture2D* Face;
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void Prox(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

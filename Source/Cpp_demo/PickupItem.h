// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UTexture2D;

UCLASS()
class CPP_DEMO_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupItem(const class FObjectInitializer& PCIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 Quantity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* ProxSphere;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UTexture2D* Icon;
	UFUNCTION(BlueprintNativeEvent)
		void Prox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UClass* Spell;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

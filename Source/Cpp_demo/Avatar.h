// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

class APickupItem;

UCLASS()
class CPP_DEMO_API AAvatar : public ACharacter
{
	GENERATED_BODY()

public:
	AAvatar();
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		float HP = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		float maxHP = 100.f;

	UPROPERTY(BlueprintReadOnly)
		int32 nMelee = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPMeleeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		float BaseMeleeDamage = 5;
	class AMeleeWeapon* MeleeWeapon;
	UFUNCTION(BlueprintCallable)
		void FinishedSwinging();

	UPROPERTY(BlueprintReadOnly)
		int32 nCast = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPSpell;
	UFUNCTION(BlueprintCallable)
		void CastSpell();
	UFUNCTION(BlueprintCallable)
		void FinishedCasting();

	UPROPERTY(BlueprintReadOnly)
		bool Jumping = false;
	UFUNCTION(BlueprintCallable)
		void FinishedJumping();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// HP
	inline float getHP() { return HP; }
	inline float getMaxHP() { return maxHP; }
	inline void decreaseHP(float amount) { HP -= amount; }

	// player control
	void MoveForward(float amount);
	void MoveBack(float amount);
	void MoveLeft(float amount);
	void MoveRight(float amount);
	void Yaw(float amount);
	void Pitch(float amount);
	void LeftMouseClicked();
	void RightMouseClicked();
	void Jump();
	
	//Inventory system
	struct Properties 
	{
		int quantity;
		UTexture2D* icon;
		Properties(): quantity(0), icon(0) {}
		Properties(int q, UTexture2D* ic): quantity(q), icon(ic) {}
	};
	TMap<FString, Properties> backpack;
	bool inventoryShowing = false;
	void ToggleInventory();
	void PickUp(APickupItem *item);
	
	// Combat
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, 
		AController* EventInstigator, AActor* DamageCauser) override;
	void MeleeAnimation();
	void CastAnimation();
	
};

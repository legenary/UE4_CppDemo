// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Avatar.generated.h"

class APickupItem;
class AMeleeWeapon;

UCLASS()
class CPP_DEMO_API AAvatar : public ACharacter
{
	GENERATED_BODY()

private:
	float HP;
	float maxHP;

public:
	// Sets default values for this character's properties
	AAvatar();

	UPROPERTY(BlueprintReadOnly, Category = AvatarProperties)
		int32 nMelee = 0;
	UFUNCTION(BlueprintCallable)
		void finishedSwinging();
	UPROPERTY(BlueprintReadOnly, Category = AvatarProperties)
		int32 nCast = 0;
	UFUNCTION(BlueprintCallable)
		void CastSpell();
	UFUNCTION(BlueprintCallable)
		void finishedCasting();
	UPROPERTY(BlueprintReadOnly, Category = AvatarProperties)
		bool Jumping = false;
	UFUNCTION(BlueprintCallable)
		void finishedJumping();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPMeleeWeapon;
	AMeleeWeapon* MeleeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		UClass* BPSpell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
		float BaseMeleeDamage = 5;


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
	
	//Inventory system
	struct Properties 
	{
		int quantity;
		UTexture2D* icon;
		Properties(): quantity(0), icon(0) {}
		Properties(int q, UTexture2D* ic): quantity(q), icon(ic) {}
	};

	TMap<FString, Properties> backpack;
	//TMap<FString, UTexture2D*> icons;
	bool inventoryShowing = false;
	void Pick(APickupItem *item);
	void ToggleInventory();

	// Melee weapon intializer
	virtual void PostInitializeComponents() override;

	// AI
	void LeftMouseClicked();
	void RightMouseClicked();
	void StartJump();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, 
		AController* EventInstigator, AActor* DamageCauser) override;
	void Melee();
	void CastAnimation();

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTERDEMO_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Spawn default gun in the hands of the character
	class AGun_Default* SpawnDefaultGun();
	//Attach a gun to a character
	void EquipGun(AGun_Default *GunToEquip);
	//Detach gun and let it fall to the ground
	void DropGun();
	//Drop current gun and equips guntoswap
	void SwapGun(AGun_Default *GunToSwap);
	void SelectButtonPressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

private:
	//Should we trace for guns to equip
	bool bShouldTraceForItems;
	int8 OverlappedItemCount;
	//Range for line tracing
	float Range = 500.f;


	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	//Character's health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Properties", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Properties", meta = (AllowPrivateAccess = "true"))
	float Health;

	//Sound when players equips gun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundBase *SoundGunEquip;

	//Line trace for guns
	void LineTracing();
	UPROPERTY(VisibleAnywhere)
	class AGun *TracedGunLastFrame;

	//The Gun that is hit by line trace (could be null)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AGun *TraceHitGun;	

public:
	//Get count of guns that are overllaped
	FORCEINLINE int8 GetOverllapedCount() const { return OverlappedItemCount; };
	void IncrementItemCount(int8 Amount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGun_Default *EquippedGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGun_Default> DefaultGunClass;

	//Check if character is dead
	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	//Get character's health percentage
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void Shoot();

};

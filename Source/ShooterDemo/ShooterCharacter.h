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
	class AGun_Default* SpawnDefaultGun();
	void EquipGun(AGun_Default *GunToEquip);
	//Detach weapon and let it fall to the ground
	void DropGun();
	//drops current gun and equips guntoswap
	void SwapGun(AGun_Default *GunToSwap);
	void SelectButtonPressed();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

private:

	bool bShouldTraceForItems;
	int8 OverlappedItemCount;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	float Range = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Properties", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Properties", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundBase *SoundGunEquip;

	//Line trace for guns
	void LineTracing();
	UPROPERTY(VisibleAnywhere)
	class AGun *TracedGunLastFrame;

	//The Gun currently hit by our trace. could be null
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AGun *TraceHitGun;	


public:
	FORCEINLINE int8 GetOverllapedCount() const { return OverlappedItemCount; };
	void IncrementItemCount(int8 Amount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGun_Default *EquippedGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGun_Default> DefaultGunClass;

	//Line trace for shooting
	FHitResult LineTrace();
	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	void Shoot();

};

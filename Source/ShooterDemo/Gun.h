// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"



UENUM(BlueprintType)
enum class EGunState : uint8 
{
	//Gun isn't equipped and lying on the ground
	EIS_PickUp UMETA(DisplayName = "PickUp"),
	//Gun is equipped by a character
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	//Gun is falling until hits the ground
	EIS_Falling UMETA(DisplayName = "Falling"),
};



UCLASS()
class SHOOTERDEMO_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	//Spawn effects if characters shoots and provide damage if something is hit
	void PullTrigger(class AShooterCharacter *Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent *GunMesh;
	//Widget with gun's properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent *WidgetDescription;
	//Component from which player can interact with a gun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent *AreaSphere;
	//Component at which player should look to see gun's properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString GunName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int BulletsCount;
	//Particles to spawn if character shoots with a gun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystem *ParticlesShoot;
	//Particles to spawn if character hits something with a gun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystem *ParticlesHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EGunState GunState;
	//Max and default attack range for a gun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float MaxRange;
	//Current attack range for a gun
	float GunRange;
	float GunDamage;
	//Sound to play if character hits something by gun shooting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundBase *SoundHitActor;
	//Set properties depending on a state
	void SetGunProperties(EGunState State);

public:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
        			AActor* OtherActor, 
                    UPrimitiveComponent* OtherComp, 
                    int32 OtherBodyIndex, 
                    bool bFromSweep, 
                    const FHitResult &SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
					AActor* OtherActor, 
                    UPrimitiveComponent* OtherComp, 
					int32 OtherBodyIndex);
	void SetGunState(EGunState NewGunState);
	UWidgetComponent* GetPickUpWidget() const { return WidgetDescription; };
	USphereComponent* GetAreaSphere() const { return AreaSphere; }
	UBoxComponent* GetBoxCollision() const { return BoxCollision; } 
	EGunState GetGunState() const { return GunState; }
	USkeletalMeshComponent *GetGunMesh() const { return GunMesh; }
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"



UENUM(BlueprintType)
enum class EGunState : uint8 
{
	EIS_PickUp UMETA(DisplayName = "PickUp"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};



UCLASS()
class SHOOTERDEMO_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
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
	//UPROPERTY(VisibleAnywhere)
	//USceneComponent *Root;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent *WidgetDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent *InRangeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString GunName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int BulletsCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystem *ParticlesShoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystem *ParticlesHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EGunState GunState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float MaxRange = 1000.f;
	float GunRange;
	float Damage = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundBase *SoundHitActor;
	//set properties depending on state
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
	FORCEINLINE UWidgetComponent* GetPickUpWidget() const { return WidgetDescription; };
	FORCEINLINE USphereComponent* GetAreaSphere() const { return InRangeComponent; }
	FORCEINLINE UBoxComponent* GetBoxCollision() const { return BoxCollision; } 
	FORCEINLINE EGunState GetGunState() const { return GunState; }
	void SetGunState(EGunState NewGunState);
	FORCEINLINE USkeletalMeshComponent *GetGunMesh() const { return GunMesh; }

};


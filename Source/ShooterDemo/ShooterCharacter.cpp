// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Gun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Gun_Default.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Gun_Default.h"
#include "Kismet/GameplayStatics.h"

#include "Components/WidgetComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	bShouldTraceForItems = false;
	//Spawn and Equip default weapon
	EquipGun(SpawnDefaultGun());	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LineTracing();
}


FHitResult AShooterCharacter::LineTrace()
{
	FVector Location;
	FRotator Rotation;
	FHitResult HitActor;
	GetController()->GetPlayerViewPoint(Location, Rotation);
	FVector EndLocation = Location + Rotation.Vector() * Range;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitActor, Location, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, QueryParams);
	return HitActor;
}


void AShooterCharacter::LineTracing()
{
	if (bShouldTraceForItems)
	{
		FVector Location;
		FRotator Rotation;
		FHitResult HitActor;
		//FCollisionQueryParams QueryParams;
		//QueryParams.AddIgnoredActor(this);
		//GetController()->GetActorEyesViewPoint(Location, Rotation);
		GetController()->GetPlayerViewPoint(Location, Rotation);
		FVector EndLocation = Location + Rotation.Vector() * Range;
		bool bsuccess = GetWorld()->LineTraceSingleByChannel(
			HitActor,
			Location,
			EndLocation,
			ECollisionChannel::ECC_Visibility
			//ECollisionChannel::ECC_GameTraceChannel1,
		//	QueryParams
		);
		DrawDebugPoint(GetWorld(), EndLocation, 50, FColor::Green);
		if (HitActor.GetActor())
		{
			TraceHitGun = Cast<AGun>(HitActor.GetActor());
			if (TraceHitGun && TraceHitGun->GetPickUpWidget())
			{
				UE_LOG(LogTemp, Error, TEXT("Hiiteed!111111111111111111111111111111"));
				TraceHitGun->GetPickUpWidget()->SetVisibility(true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("NO WIDGET"));
			}

			if (TracedGunLastFrame)
			{
				if (TracedGunLastFrame != TraceHitGun)
				{
					TracedGunLastFrame->GetPickUpWidget()->SetVisibility(false);
				}
			}

			//store last seen gun
			TracedGunLastFrame = TraceHitGun;
		}
		if (bsuccess)
		{
			DrawDebugCamera(GetWorld(), EndLocation, Rotation, 100, 2, FColor::Red, true);

			UE_LOG(LogTemp, Warning, TEXT("hitted %s"), *HitActor.GetActor()->GetName());
		}

	}
	else if (TracedGunLastFrame)
	{
		TracedGunLastFrame->GetPickUpWidget()->SetVisibility(false);
	}
	//return HitActor;
}
// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("EquipGun"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SelectButtonPressed);

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}


void AShooterCharacter::SelectButtonPressed()
{
	if (TraceHitGun)
	{
		auto TracedHitGun = Cast<AGun_Default>(TraceHitGun);
		SwapGun(TracedHitGun);
	}
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}


void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}


AGun_Default* AShooterCharacter::SpawnDefaultGun()
{
	if (DefaultGunClass)
	{
		AGun_Default *SpawnGun =  GetWorld()->SpawnActor<AGun_Default>(DefaultGunClass);
		//SpawnGun->SetOwner(this);
		return SpawnGun;
	}
	return nullptr;
}


float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Error, TEXT("Health left %f"), Health);
	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

void AShooterCharacter::EquipGun(AGun_Default *GunToEquip)
{
	if (GunToEquip)
	{
		//GunToEquip->GetAreaSphere()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		//GunToEquip->GetBoxCollision()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		const USkeletalMeshSocket *RightHandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(GunToEquip, GetMesh());
		}
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundGunEquip, GetOwner()->GetActorLocation());
		EquippedGun = GunToEquip;
		EquippedGun->SetGunState(EGunState::EIS_Equipped);
	}

}

void AShooterCharacter::DropGun()
{
	if (EquippedGun)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedGun->DetachFromActor(DetachmentTransformRules);
		//EquippedGun->GetGunMesh()->DetachFromComponent(DetachmentTransformRules);

		EquippedGun->SetGunState(EGunState::EIS_Falling);
		EquippedGun->ThrowGun();
	}
}

void AShooterCharacter::SwapGun(AGun_Default *GunToSwap)
{
	DropGun();
	EquipGun(GunToSwap);
	TraceHitGun = nullptr;
	TracedGunLastFrame = nullptr;
}

void AShooterCharacter::IncrementItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <=0 )
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

void AShooterCharacter::Shoot()
{
	EquippedGun->PullTrigger(this);
}

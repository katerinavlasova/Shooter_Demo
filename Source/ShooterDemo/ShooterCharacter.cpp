// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Gun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Gun_Default.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gun_Default.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"

#include "ShooterDemoGameModeBase.h"


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
	//Hide bone with a gun in a character's asset
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

void AShooterCharacter::LineTracing()
{
	//Check if gun is within player's range
	if (bShouldTraceForItems)
	{
		FVector Location;
		FRotator Rotation;
		FHitResult HitActor;
		GetController()->GetPlayerViewPoint(Location, Rotation);
		FVector EndLocation = Location + Rotation.Vector() * Range;

		bool bsuccess = GetWorld()->LineTraceSingleByChannel(
			HitActor,
			Location,
			EndLocation,
			ECollisionChannel::ECC_Visibility
		);
		//If player hits an object
		if (HitActor.GetActor())
		{
			TraceHitGun = Cast<AGun>(HitActor.GetActor());
			//If player traced a gun, show it's widget with description
			if (TraceHitGun && TraceHitGun->GetPickUpWidget())
			{
				TraceHitGun->GetPickUpWidget()->SetVisibility(true);
			}

			//if player traced a gun in last frame
			if (TracedGunLastFrame)
			{
				//if player's traced gun isn't the one player traced in last frame
				if (TracedGunLastFrame != TraceHitGun)
				{
					//hide untraced gun's description
					TracedGunLastFrame->GetPickUpWidget()->SetVisibility(false);
				}
			}

			//Store traced gun
			TracedGunLastFrame = TraceHitGun;
		}

	}
	//Hide last traced gun's description if we no longer trace anything anymore
	else if (TracedGunLastFrame)
	{
		TracedGunLastFrame->GetPickUpWidget()->SetVisibility(false);
	}
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
		return SpawnGun;
	}
	return nullptr;
}


float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	//Apply damage to a character
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
	  	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AShooterDemoGameModeBase *GameMode = GetWorld()->GetAuthGameMode<AShooterDemoGameModeBase>();
		if (GameMode)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
	}
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
		//Attach gun to the player
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

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}


void AShooterCharacter::Shoot()
{
	EquippedGun->PullTrigger(this);
}

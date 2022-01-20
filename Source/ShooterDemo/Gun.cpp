// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	WidgetDescription = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetDescription->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InRangeToSee"));
	AreaSphere->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	 BoxCollision->SetCollisionResponseToChannel(
	 	ECollisionChannel::ECC_Visibility,
	 	ECollisionResponse::ECR_Block
	 );

	//Default gun name
	GunName = FString("Shotgun");
	GunRange = MaxRange;
	GunDamage = 100.f;
	MaxRange = 1000.f;
	BulletsCount = 100;
	GunState = EGunState::EIS_PickUp;
	SetGunProperties(GunState);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	WidgetDescription->SetVisibility(false);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AGun::OnSphereEndOverlap);
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,  AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AShooterCharacter *ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->IncrementItemCount(1);
		}
	}
}


void AGun::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShooterCharacter *ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter)
	{
		ShooterCharacter->IncrementItemCount(-1);
	}
}


void AGun::SetGunProperties(EGunState State)
{
	switch (State)
	{

		case EGunState::EIS_PickUp:

			//Set gun mesh properties
			GunMesh->SetSimulatePhysics(false);
			GunMesh->SetEnableGravity(false);
			GunMesh->SetVisibility(true);
			GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			//Set box collision properties
			BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;
		
		case EGunState::EIS_Equipped:

			//Set gun mesh properties
			GetPickUpWidget()->SetVisibility(false);
			GunMesh->SetSimulatePhysics(false);
			GunMesh->SetEnableGravity(false);
			GunMesh->SetVisibility(true);
			GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set box collision properties
			BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;

		case EGunState::EIS_Falling:

			//Set gun mesh properties
			GunMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GunMesh->SetSimulatePhysics(true);
			GunMesh->SetEnableGravity(true);
			GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			GunMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

			//Set area sphere properties
			AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//Set box collision properties
			BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
	}
}


void AGun::SetGunState(EGunState NewGunState)
{
	GunState = NewGunState;
	SetGunProperties(NewGunState);

}


void AGun::PullTrigger(AShooterCharacter *Character)
{
	UGameplayStatics::SpawnEmitterAttached(ParticlesShoot, GunMesh, TEXT("MuzzleFlashSocket"));
	FVector Location;
	FRotator Rotation;
	FHitResult HitResult;
	Character->GetController()->GetPlayerViewPoint(Location, Rotation);
	FVector EndLocation = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Character);
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, QueryParams);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundHitActor, Location, Rotation);

	//If gun fire hits something, spawn particles and make that object take damage
	if (bSuccess)
	{
		FVector ShootDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesHit, HitResult.Location, ShootDirection.Rotation());
		if (HitResult.GetActor() != nullptr)
		{
			FPointDamageEvent DamageEventShoot(GunDamage, HitResult, ShootDirection, nullptr);
			HitResult.GetActor()->TakeDamage(GunDamage, DamageEventShoot, Character->GetController(), this);
		}
	}
	bSuccess = false;
}

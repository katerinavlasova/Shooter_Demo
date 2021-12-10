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
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(Root);
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);
	//GunMesh->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	WidgetDescription = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	InRangeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InRangeToSee"));

	//BoxCollision->SetupAttachment(GetRootComponent());
	//BoxCollision->SetupAttachment(GunMesh);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	 BoxCollision->SetCollisionResponseToChannel(
	 	ECollisionChannel::ECC_Visibility,
	 	ECollisionResponse::ECR_Block
	 );


	//WidgetDescription->SetupAttachment(GetRootComponent());
	WidgetDescription->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);
	//InRangeComponent->SetupAttachment(GetRootComponent());
	InRangeComponent->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform);
	GunName = FString("Shotgun");
	GunRange = MaxRange;
	BulletsCount = 100;
	GunState = EGunState::EIS_PickUp;
	SetGunProperties(GunState);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	WidgetDescription->SetVisibility(false);
	InRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnSphereOverlap);
	InRangeComponent->OnComponentEndOverlap.AddDynamic(this, &AGun::OnSphereEndOverlap);
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
		UE_LOG(LogTemp, Error, TEXT("void AGun::SetGunProperties(EGunState State)"));

		case EGunState::EIS_PickUp:
			//this->SetOwner(nullptr);
			GunMesh->SetSimulatePhysics(false);
			GunMesh->SetEnableGravity(false);
			GunMesh->SetVisibility(true);
			GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//set area sphere properties
			InRangeComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			InRangeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//set box collision properties
			BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;
		case EGunState::EIS_Equipped:
			GetPickUpWidget()->SetVisibility(false);
			GunMesh->SetSimulatePhysics(false);
			GunMesh->SetEnableGravity(false);
			GunMesh->SetVisibility(true);
			GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//set area sphere properties
			InRangeComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			InRangeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//set box collision properties
			BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			//BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EGunState::EIS_Falling:
			GunMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GunMesh->SetSimulatePhysics(true);
			GunMesh->SetEnableGravity(true);
			GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			GunMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
			//set area sphere properties
			InRangeComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			InRangeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//set box collision properties
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
	UE_LOG(LogTemp, Warning, TEXT("Piy piy"));
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
	if (bSuccess)
	{
		FVector ShootDirection = -Rotation.Vector();
		UE_LOG(LogTemp, Warning, TEXT("spawning??? %s"), *HitResult.GetActor()->GetName());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesHit, HitResult.Location, ShootDirection.Rotation());//HitResult.GetActor()->GetActorRotation());
		//UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundHitActor, Location, Rotation);
		if (HitResult.GetActor() != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("send damage"));
			FPointDamageEvent DamageEventShoot(Damage, HitResult, ShootDirection, nullptr);
			HitResult.GetActor()->TakeDamage(Damage, DamageEventShoot, Character->GetController(), this);
		}
	}
	bSuccess = false;
//	APawn *Owner = Cast<APawn>(GetOwner());

//	Owner->viewpo
//	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ParticlesHit, )
}

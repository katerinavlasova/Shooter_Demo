// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_Default.h"

AGun_Default::AGun_Default() :
    ThrowGunTime(0.7f),
    bFalling(false)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGun_Default::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //keep the gun upright
    if (GetGunState() == EGunState::EIS_Falling && bFalling)
    {
        FRotator MeshRotation{ 0.f, GetGunMesh()->GetComponentRotation().Yaw, 0.f };
        GetGunMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
    }
}



void AGun_Default::ThrowGun()
{
    FRotator MeshRotation{ 0.f, GetGunMesh()->GetComponentRotation().Yaw, 0.f };
    GetGunMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::ResetPhysics);

    FVector MeshForward{ GetGunMesh()->GetForwardVector() };
    FVector ImpulseDirection{ GetGunMesh()->GetRightVector() };
    //direction to throw the gun
    ImpulseDirection = ImpulseDirection.RotateAngleAxis(-20.f, MeshForward);

    float RandomRotation{ 30.f };
    ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
    ImpulseDirection *= 10'000.f;
    GetGunMesh()->AddImpulse(ImpulseDirection);

    bFalling = true;
    GetWorldTimerManager().SetTimer(ThrowGunTimer, this, &AGun_Default::StopFalling, ThrowGunTime);
}

void AGun_Default::StopFalling()
{
    bFalling = false;
    SetGunState(EGunState::EIS_PickUp);
}
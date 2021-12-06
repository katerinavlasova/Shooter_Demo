// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Gun_Default.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERDEMO_API AGun_Default : public AGun
{
	GENERATED_BODY()

public:
	AGun_Default();
	virtual void Tick(float DeltaTime) override;
	// add impulse to the gun
	void ThrowGun();
protected:
	FTimerHandle ThrowGunTimer;
	void StopFalling();
private:
	float ThrowGunTime;
	bool bFalling;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	virtual void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *ProjSpawnPoint;

	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase *MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase *ImpactSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem *HitParticles;

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000.f;

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	bool GunTrace(FHitResult &OutHit, FVector &ShotDirection);

	AController *GetController() const;
};

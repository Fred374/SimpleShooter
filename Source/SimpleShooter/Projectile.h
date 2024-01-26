// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SIMPLESHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent *MoveComp;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UParticleSystem *HitParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USoundBase *HitSound;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent *ParticleComp;

	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere)
	float DamageRadius = 100.f;
};

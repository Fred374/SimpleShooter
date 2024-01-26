// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = Mesh;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ParticleComp->SetupAttachment(RootComponent);

	MoveComp->InitialSpeed = 1200;
	MoveComp->MaxSpeed = 1200;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	AActor *MyOwner = GetOwner();
	if (MyOwner)
	{
		AController *MyInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		if (OtherActor && OtherActor != this && OtherActor != MyOwner)
		{
			TArray<AActor *> IgnoreActors;
			IgnoreActors.Add(MyOwner->GetOwner());
			UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageTypeClass, IgnoreActors, MyOwner, MyInstigator);
		}
	}

	if (HitParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
	if (HitSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	Destroy();
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetupAttachment(Root);

	ProjSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult &OutHit, FVector &ShotDirection)
{
	AController *OwnerController = GetController();
	if (OwnerController)
	{
		FVector Location;
		FRotator Rotation;
		OwnerController->GetPlayerViewPoint(Location, Rotation);
		// Rotation.Yaw -= 2;
		FVector End = Location + Rotation.Vector() * MaxRange;
		ShotDirection = -Rotation.Vector();
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		// return GetWorld()->LineTraceSingleByChannel(OutHit, ProjSpawnPoint->GetComponentLocation(), End, ECollisionChannel::ECC_GameTraceChannel1);
		return GetWorld()->LineTraceSingleByChannel(OutHit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
	}
	return false;
}

AController *AGun::GetController() const
{
	APawn *OwnerPawn = (APawn *)GetOwner();
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	FHitResult OutHit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(OutHit, ShotDirection);
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, OutHit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, OutHit.Location);
		if (OutHit.GetActor())
		{
			FPointDamageEvent DamageEvent(Damage, OutHit, ShotDirection, nullptr);
			AController *OwnerController = GetController();
			OutHit.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, GetOwner());
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Launcher.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Projectile.h"

// Sets default values
ALauncher::ALauncher()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // SetRootComponent(Root);

    // Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    // Mesh->SetupAttachment(Root);

    // ProjSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
    // ProjSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ALauncher::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ALauncher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

AController *ALauncher::GetController() const
{
    APawn *OwnerPawn = (APawn *)GetOwner();
    if (OwnerPawn == nullptr)
        return nullptr;
    return OwnerPawn->GetController();
}

void ALauncher::PullTrigger()
{
    AController *OwnerController = GetController();
    UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
    UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
    FVector Location;
    FRotator Rotation;
    FVector ShotDirection;

    if (OwnerController)
    {
        OwnerController->GetPlayerViewPoint(Location, Rotation);
        Rotation.Pitch +=2;
        Rotation.Yaw += 1;
    }

    UE_LOG(LogTemp, Error, TEXT("SHOOTING"));
    AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(ProjClass, ProjSpawnPoint->GetComponentLocation(), Rotation);
    Projectile->SetOwner(this);

    // UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, OutHit.Location, ShotDirection.Rotation());
    // UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, OutHit.Location);
    // if (OutHit.GetActor())
    // {
    //     FPointDamageEvent DamageEvent(Damage, OutHit, ShotDirection, nullptr);
    //     AController *OwnerController = GetController();
    //     OutHit.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, GetOwner());
    // }
}
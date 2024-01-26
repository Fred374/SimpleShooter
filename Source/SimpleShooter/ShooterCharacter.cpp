// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

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

	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (GunClasses.Num() > 0)
	{
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		for (int i = 0; i < GunClasses.Num(); i++)
		{
			Guns.Add(GetWorld()->SpawnActor<AGun>(GunClasses[i]));
			Guns[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			Guns[i]->SetOwner(this);
			Guns[i]->SetActorHiddenInGame(true);
		}
		Guns[GunIndex]->SetActorHiddenInGame(false);
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AShooterCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AShooterCharacter::MoveRight);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookUp);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Turn);
		EnhancedInputComponent->BindAction(LookUpRateAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookUpRate);
		EnhancedInputComponent->BindAction(TurnRateAction, ETriggerEvent::Triggered, this, &AShooterCharacter::TurnRate);
		EnhancedInputComponent->BindAction(PullTriggerAction, ETriggerEvent::Started, this, &AShooterCharacter::PullTrigger);
		EnhancedInputComponent->BindAction(GunScrollAction, ETriggerEvent::Started, this, &AShooterCharacter::GunScroll);
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
		ASimpleShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::MoveForward(const FInputActionValue &AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue.Get<float>());
}

void AShooterCharacter::MoveRight(const FInputActionValue &AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue.Get<float>());
}

void AShooterCharacter::LookUp(const FInputActionValue &AxisValue)
{
	AddControllerPitchInput(AxisValue.Get<float>());
}

void AShooterCharacter::Turn(const FInputActionValue &AxisValue)
{
	AddControllerYawInput(AxisValue.Get<float>());
}

void AShooterCharacter::LookUpRate(const FInputActionValue &AxisValue)
{
	AddControllerPitchInput(AxisValue.Get<float>() * RotRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::TurnRate(const FInputActionValue &AxisValue)
{
	AddControllerYawInput(AxisValue.Get<float>() * RotRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::GunScroll(const FInputActionValue &AxisValue)
{
	float val = AxisValue.Get<float>();
	Guns[GunIndex]->SetActorHiddenInGame(true);
	if (val > 0 && Guns.Num() - 1 > GunIndex)
	{
		GunIndex++;
	}
	else if (val > 0)
	{
		GunIndex = 0;
	}
	else if (val < 0 && GunIndex == 0)
	{
		GunIndex = Guns.Num() - 1;
	}
	else
	{
		GunIndex--;
	}
	Guns[GunIndex]->SetActorHiddenInGame(false);
}

void AShooterCharacter::PullTrigger()
{
	FString name = Guns[GunIndex]->GetClass()->GetName();
	UE_LOG(LogTemp, Error, TEXT("%s"), *name);
	Guns[GunIndex]->PullTrigger();
}
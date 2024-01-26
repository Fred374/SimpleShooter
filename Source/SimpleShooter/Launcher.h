// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Launcher.generated.h"

UCLASS()
class SIMPLESHOOTER_API ALauncher : public AGun
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALauncher();

	void PullTrigger() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	AController *GetController() const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> ProjClass;
};

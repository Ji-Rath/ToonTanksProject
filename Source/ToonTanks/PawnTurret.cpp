// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

APawnTurret::APawnTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APawnTurret::CheckFireCondition()
{
	//If player is dead, then stop!
	if(!Player) {return;}
	// If player is in range, shoot
	if(GetDistanceToPlayer() <= FireRange && Player->GetPlayerAlive())
	{
		// fire weapon
		Fire();
	}
}

float APawnTurret::GetDistanceToPlayer()
{
	if(!Player) {return 0.f;}

	float Distance = ((Player->GetActorLocation()-GetActorLocation()).Size());

	return Distance;
}

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true, false);

	Player = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();

	//Handle destruction-

	Destroy();
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player || GetDistanceToPlayer() > FireRange)
	{
		return;
	}
	
	RotateTurret(Player->GetActorLocation());
}

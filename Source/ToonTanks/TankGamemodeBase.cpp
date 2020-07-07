// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGamemodeBase.h"
#include "PawnTank.h"
#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PlayerControllerBase.h"

void ATankGamemodeBase::HandleGameStart()
{
	//Start countdown, activate turrets, check pawns
	GameStart();

	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableHandle;
		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerController, &APlayerControllerBase::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
	}
}

void ATankGamemodeBase::HandleGameEnd(bool PlayerWon)
{
	//Check if all turrets are destroyed, show win result

	//else if player is destroyed, show lose result

	//Call blueprint GameEnd()
	GameEnd(PlayerWon);
}

int32 ATankGamemodeBase::GetTurretCount()
{
	TSubclassOf<APawnTurret> ClassToFind;
	ClassToFind = APawnTurret::StaticClass();

	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, TurretActors);

	return TurretActors.Num();
}

void ATankGamemodeBase::BeginPlay()
{
	Super::BeginPlay();

	Turrets = GetTurretCount();

	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	HandleGameStart();
	//Get win/lose conditions

	//call HandleGameStart to initialize start countdown, activate turrets, check pawns
}

void ATankGamemodeBase::ActorDied(AActor* DeadActor)
{
	//If turret died, tally result

	if (DeadActor == PlayerTank)
	{
		PlayerTank->PawnDestroyed();
		if (PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
			GameEnd(false);
		}
	}
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		DestroyedTurret->PawnDestroyed();
		Turrets--;

		if (Turrets == 0)
		{
			HandleGameEnd(true);
		}
	}
	//If player dead, go to lose result
}

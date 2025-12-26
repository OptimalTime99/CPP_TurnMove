// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Engine/Engine.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		RandomActionTimer,
		this,
		&AMyActor::RandomAction,
		1.0f,
		true
	);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyActor::Turn()
{
	const float RandPitch = FMath::RandRange(-180.f, 180.f);
	const float RandYaw   = FMath::RandRange(-180.f, 180.f);
	const float RandRoll  = FMath::RandRange(-180.f, 180.f);
	FRotator NewRot(RandPitch, RandYaw, RandRoll);
	SetActorRotation(NewRot);
}


void AMyActor::Move()
{
	const FVector RandomDir = FMath::VRand();
	const float Distance = FMath::RandRange(100.f, 500.f);

	FVector NewLocation = GetActorLocation() + RandomDir * Distance;
	SetActorLocation(NewLocation, true);
}

void AMyActor::RandomAction()
{
	Turn();
	Move();
	++ActionCount;

	FVector Location = GetActorLocation();
	FRotator Rotator = GetActorRotation();

	const FString LocStr = FString::Printf(
		TEXT("현재 좌표: X(%.0f) / Y(%.0f) / Z(%.0f)"),
		Location.X, Location.Y, Location.Z
	);

	const FString RotStr = FString::Printf(
		TEXT("현재 회전: X(%.0f) / Y(%.0f) / Z(%.0f)"),
		Rotator.Pitch, Rotator.Yaw, Rotator.Roll
	);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,  LocStr);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,   RotStr);
	}

	if (ActionCount >= MaxActionCount)
	{
		GetWorldTimerManager().ClearTimer(RandomActionTimer);
	}
}
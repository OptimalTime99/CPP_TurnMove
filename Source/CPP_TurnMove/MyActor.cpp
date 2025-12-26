// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "MyActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

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

	auto Cube = FindComponentByClass<UStaticMeshComponent>();

	if (Cube)
	{
		auto Material = Cube->GetMaterial(0);

		if (Material)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
			Cube->SetMaterial(0, DynamicMaterial);
		}
	}
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//float blend = 0.5f + FMath::Cos(GetWorld()->TimeSeconds) / 2;
	//DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
}

void AMyActor::RandomAction()
{
	Turn();
	Move();
	ChangeColor();
	PrintTotalMove();
}

void AMyActor::Turn()
{
	const float RandPitch = FMath::RandRange(-180.f, 180.f);
	const float RandYaw   = FMath::RandRange(-180.f, 180.f);
	const float RandRoll  = FMath::RandRange(-180.f, 180.f);
	FRotator NewRot(RandPitch, RandYaw, RandRoll);
	SetActorRotation(NewRot);

	FRotator Rotator = GetActorRotation();

	const FString RotStr = FString::Printf(
		TEXT("현재 회전: X(%.0f) / Y(%.0f) / Z(%.0f)"),
		Rotator.Pitch, Rotator.Yaw, Rotator.Roll
	);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, RotStr);
	}
}


void AMyActor::Move()
{
	const float DX = FMath::RandRange(-100.f, 100.f);
	const float DY = FMath::RandRange(-100.f, 100.f);
	const float DZ = FMath::RandRange(-50.f, 100.f);

	FVector Offset(DX, DY, DZ);
	SetActorLocation(GetActorLocation() + Offset, true);

	static FVector PrevLocation = InitialLocation;
	FVector CurrentLocation = GetActorLocation();

	TotalDistance += FVector::Dist(PrevLocation, CurrentLocation);
	PrevLocation = CurrentLocation;

	++ActionCount;

	FVector Location = GetActorLocation();

	const FString LocStr = FString::Printf(
		TEXT("현재 좌표: X(%.0f) / Y(%.0f) / Z(%.0f)"),
		Location.X, Location.Y, Location.Z
	);

	const FString MoveCountStr = FString::Printf(
		TEXT("현재 이동 횟수: %d"),
		ActionCount
	);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, LocStr);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, MoveCountStr);
	}
}

void AMyActor::ChangeColor()
{
	if (DynamicMaterial)
	{
		// FMath::RandRange(0, 1) 또는 FMath::FRand() < 0.5f 사용
		// 0 또는 1을 무작위로 선택 (50% 확률)
		float blend = (FMath::RandRange(0, 1) == 0) ? 0.0f : 1.0f;

		const FString BlendValue = FString::Printf(
			TEXT("Current Blend: %f"),
			blend
		);

		// 머티리얼 파라미터 업데이트
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, BlendValue);
	}
}

void AMyActor::PrintTotalMove()
{
	if (ActionCount >= MaxActionCount)
	{
		GetWorldTimerManager().ClearTimer(RandomActionTimer);

		const FString DistanceStr = FString::Printf(
			TEXT("총 이동 거리: %.1f"),
			TotalDistance
		);

		const FString FinalCountStr = FString::Printf(
			TEXT("총 이동 횟수: %d"),
			ActionCount
		);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, DistanceStr);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FinalCountStr);
		}

	}
}


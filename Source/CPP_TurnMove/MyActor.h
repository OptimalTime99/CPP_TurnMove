// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class CPP_TURNMOVE_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void RandomAction();
	void Turn();
	void Move();
	void ChangeColor();
	void PrintTotalMove();

private:
	FTimerHandle RandomActionTimer;

	int32 ActionCount = 0;
	const int32 MaxActionCount = 10;
	FVector InitialLocation = FVector(0.f, 50.f, 0.f);
	float TotalDistance = 0.f;

	UMaterialInstanceDynamic* DynamicMaterial;
};

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
	// 생성자
	AMyActor();

protected:
	// 액터 생성 시 호출한다.
	virtual void BeginPlay() override;

public:	
	// 매 프레임 호출한다.
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

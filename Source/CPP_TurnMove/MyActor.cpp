// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "MyActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

// 생성자
AMyActor::AMyActor()
{
 	// 매 프레임마다 Tick() 호출 비활성화
	PrimaryActorTick.bCanEverTick = false;

}

// 액터 생성 시 호출한다.
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// 1초마다 RandomAction 함수 호출
	// 매개변수: (타이머 식별자, 타이머를 실행할 객체, 1초마다 호출될 멤버 함수, 1초, 계속 반복 여부)
	GetWorldTimerManager().SetTimer(
		RandomActionTimer,
		this,
		&AMyActor::RandomAction,
		1.0f,
		true
	);

	// FindComponentByClass<UStaticMeshComponent>(): 이 액터가 가지고 있는 컴포넌트 중 UStaticMeshComponent 타입 하나를 찾아 그 주소를 반환
	UStaticMeshComponent* Cube = FindComponentByClass<UStaticMeshComponent>();

	if (Cube)
	{
		// Cube 메시의 0번 머티리얼 슬롯에 설정된 머티리얼을 가져와서, 그 주소를 Material 포인터로 받는다
		UMaterialInterface* Material = Cube->GetMaterial(0);

		if (Material)
		{
			// 기존 Material을 기반으로 한 동적 머티리얼 인스턴스(DMI) 생성
			// nullptr: Outer 지정 안 함
			// → 이 오브젝트만 사용하는 독립적인 머티리얼 복사본
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, nullptr);

			// Cube의 0번 머티리얼 슬롯에 DMI 장착
			Cube->SetMaterial(0, DynamicMaterial);
		}
	}
}

// 매 프레임 호출한다.
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FMath::Cos(GetWorld()->TimeSeconds): 월드 시작 이후 경과 시간(초)을 입력으로 하는 코사인 함수. 출력 범위 [-1, 1]
	// / 2해서 [-0.5, 0.5]로 범위 축소하고 0.5f를 더해 [0.0, 1.0] 범위 이동
	// → 시간에 따라 0과 1을 부드럽게 왕복하는 값
	//float blend = 0.5f + FMath::Cos(GetWorld()->TimeSeconds) / 2;
	

	// 머티리얼에 정의된 Scalar Parameter 이름이 Blend인 값을 방금 계산한 blend 값으로 실시간 갱신
	// → 색상 보간 효과
	//DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
}

void AMyActor::RandomAction()
{
	Turn();
	Move();
	ChangeColor();
	PrintTotalMove();
}

// 액터 회전 함수
void AMyActor::Turn()
{
	// -180 ~ 180 사이 랜덤 Pitch(상하 회전) 값 생성
	const float RandPitch = FMath::RandRange(-180.f, 180.f);

	// -180 ~ 180 사이 랜덤 Yaw(좌우 회전) 값 생성
	const float RandYaw   = FMath::RandRange(-180.f, 180.f);

	// -180 ~ 180 사이 랜덤 Roll(기울기) 값 생성
	const float RandRoll  = FMath::RandRange(-180.f, 180.f);

	// Pitch, Yaw, Roll 순서로 값을 묶어 회전 값 생성
	FRotator Rotator(RandPitch, RandYaw, RandRoll);

	// 이 액터의 월드 회전을 NewRot으로 설정
	SetActorRotation(Rotator);

	// TEXT(): UE에서 쓰는 와이드 문자열 매크로
	const FString RotateStr = FString::Printf(
		TEXT("현재 회전: Pitch(%.0f) / Yaw(%.0f) / Roll(%.0f)"),
		Rotator.Pitch, Rotator.Yaw, Rotator.Roll
	);

	if (GEngine) // GEngine 포인터가 유효한지 체크(에디터/게임 상황에 따라 null일 수 있어 크래시 방지)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, RotateStr);
	}
}

// 액터 이동 함수
void AMyActor::Move()
{
	// -100 ~ 100 사이 랜덤 X축 이동량 값 생성
	const float RandX = FMath::RandRange(-100.f, 100.f);

	// -100 ~ 100 사이 랜덤 Y축 이동량 값 생성
	const float RandY = FMath::RandRange(-100.f, 100.f);

	// -50 ~ 100 사이 랜덤 Z축 이동량 값 생성
	const float RandZ = FMath::RandRange(-50.f, 100.f);

	// 이동량을 하나의 벡터로 묶음
	FVector Offset(RandX, RandY, RandZ);

	// 현재 위치 + 이동량으로 액터 위치 변경
	// true → 충돌이 있으면 이동 실패하거나 슬라이딩
	SetActorLocation(GetActorLocation() + Offset, true);

	// 이전 위치 저장용 정적 변수
	static FVector PrevLocation = InitialLocation;

	// 이동이 끝난 뒤 현재 위치 저장
	FVector CurrentLocation = GetActorLocation();


	// FVector::Dist(A, B): 두 위치(Vector) 사이의 직선 거리 계산
	TotalDistance += FVector::Dist(PrevLocation, CurrentLocation);

	// 다음 호출을 위해 이전 위치 갱신
	PrevLocation = CurrentLocation;

	// 이동 횟수 1 증가
	++ActionCount;

	const FString LocStr = FString::Printf(
		TEXT("현재 좌표: X(%.0f) / Y(%.0f) / Z(%.0f)"),
		CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z
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

// 50% 확률로 액터 색상 변경 함수
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

// MaxActionCount회(10회) 이동 후 총 이동 거리, 총 이동 횟수 출력 함수
void AMyActor::PrintTotalMove()
{
	if (ActionCount >= MaxActionCount)
	{
		// RandomActionTimer로 등록된 타이머를 중지하고 제거한다.
		// GetWorldTimerManager(): 현재 월드가 관리하는 타이머 관리자
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


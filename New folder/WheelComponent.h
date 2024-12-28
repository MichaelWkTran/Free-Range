// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WheelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHIKNRAGEGAME_API UWheelComponent : public USceneComponent
{
	GENERATED_BODY()

#pragma region Constructors, Destructors & Events
public:	
	// Sets default values for this component's properties
	UWheelComponent();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion

public:
	USceneComponent* WheelComponent;
	FVector WheelOffset;
	float WheelAngularSpeed;
	float WheelPosition;

	UPROPERTY(Category = "Rendering", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float WheelAngularDamping;
	UPROPERTY(Category = "Rendering", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAxis::Type> RotationAxis;
	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Input;
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRuntimeFloatCurve Thrust;
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRuntimeFloatCurve Break;
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRuntimeFloatCurve Reverse;
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Drag;
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRuntimeFloatCurve Grip;
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpringLength;
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float WheelRadius;
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Suspension;
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SuspensionDamping;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSuspensionDebugEnabled;
#endif
};
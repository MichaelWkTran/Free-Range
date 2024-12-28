// Fill out your copyright notice in the Description page of Project Settings.


#include "WheelComponent.h"

// Sets default values for this component's properties
UWheelComponent::UWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UWheelComponent::BeginPlay()
{
    // Get Wheel Component
    WheelComponent = GetChildComponent(0); if (IsValid(WheelComponent)) WheelOffset = WheelComponent->GetRelativeLocation();
}

// Called every frame
void UWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UPrimitiveComponent* BodyMesh = GetOwner()->FindComponentByClass<UPrimitiveComponent>();

    //Update Wheel Component
    if (IsValid(WheelComponent))
    {
        // Set the world location of the wheel mesh
        WheelComponent->SetWorldLocation(
            GetComponentLocation() - (GetUpVector() * (WheelPosition - WheelRadius)),
            false, nullptr, ETeleportType::TeleportPhysics
        );
        WheelComponent->AddLocalOffset(WheelOffset);

        // Rotate Wheel
        float WheelFinalSpeed = -WheelAngularSpeed / WheelRadius;
        WheelComponent->AddRelativeRotation(FRotator(
            RotationAxis == EAxis::X ? WheelFinalSpeed : 0.0f,
            RotationAxis == EAxis::Y ? WheelFinalSpeed : 0.0f,
            RotationAxis == EAxis::Z ? WheelFinalSpeed : 0.0f));

        // Apply Wheel Angular Damping
        WheelAngularSpeed -= WheelAngularSpeed * WheelAngularDamping;
    }

    // Prepare for the line trace
    FHitResult Hit;
    FCollisionQueryParams WheelCollisionQuery;
    WheelCollisionQuery.AddIgnoredComponent(BodyMesh);

    float TotalSpringLength = SpringLength + WheelRadius;
    FVector Start = GetComponentLocation() + GetUpVector() * WheelRadius;
    FVector End = GetComponentLocation() - GetUpVector() * TotalSpringLength;

#if WITH_EDITOR
    // Draw debug visualizations
    if (bSuspensionDebugEnabled)
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0, 5);
        DrawDebugSphere(GetWorld(), GetComponentLocation(), WheelRadius, 16, FColor::Green, false, -1, 0, 2.0f);
    }
#endif
    
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, WheelCollisionQuery))
    {
        FVector WheelForce;

        // Get the wheel's velocity in world space and transform it to local space
        FVector LocalVelocity = GetComponentTransform().InverseTransformVector(
            BodyMesh->GetPhysicsLinearVelocityAtPoint(GetComponentLocation()));
        float Speed = LocalVelocity.Size();

        // Set the position of the wheel
        WheelPosition = FMath::GetMappedRangeValueUnclamped(
            FVector2D(0, TotalSpringLength + WheelRadius),
            FVector2D(-WheelRadius, TotalSpringLength),
            Hit.Distance
        );

        // Calculate and apply forces
        FRuntimeFloatCurve* UsedForce =
            (LocalVelocity.X > 0.0f && Input > 0.0f) ? &Thrust :
            ((LocalVelocity.X > 0.0f && Input < 0.0f) || (LocalVelocity.X < 0.0f && Input > 0.0f)) ? &Break :
            &Reverse;
        WheelForce.X = Input == 0.0f ? -LocalVelocity.X * Drag : FMath::Sign(Input) * UsedForce->GetRichCurveConst()->Eval(FMath::Abs(LocalVelocity.X));
        WheelForce.Y = -LocalVelocity.Y * Grip.GetRichCurveConst()->Eval(Speed);
        WheelForce.Z = (TotalSpringLength - WheelPosition) * Suspension - LocalVelocity.Z * SuspensionDamping;

        BodyMesh->AddForceAtLocation(
            BodyMesh->GetMass() * GetComponentTransform().TransformVector(WheelForce), GetComponentLocation());

        //Update Wheel Angular Speed
        WheelAngularSpeed = LocalVelocity.X;

#if WITH_EDITOR
        // Draw the hit point
        if (bSuspensionDebugEnabled) DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Green, false, -1, 0);
#endif
    }
}


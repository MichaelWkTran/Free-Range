// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChicken.h"
#include "ChikenRageGameGamemode.h"
#include "Kismet/GameplayStatics.h"

#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Sound/SoundBase.h"

#include "Misc/OutputDeviceNull.h" //Used to call blueprint functions in C++


// Sets default values
APlayerChicken::APlayerChicken()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create SphereComponent and set it as the root component
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); {
        SphereComponent->SetSimulatePhysics(true);
        SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
        SphereComponent->SetUseCCD(true);
    }
    RootComponent = SphereComponent;

    // Create and attach the skeletal mesh component
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    // Create and attach the left wing component
    LeftWingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LeftWing"));
    LeftWingComponent->SetupAttachment(RootComponent);

    // Create and attach the right wing component
    RightWingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RightWing"));
    RightWingComponent->SetupAttachment(RootComponent);

    // Create and attach the camera boom (spring arm) component
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); {
        CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
        CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
    }
    CameraBoom->SetupAttachment(RootComponent);
    
    // Create and attach the follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); {
        FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
    }
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
}

// Called when the game starts or when spawned
void APlayerChicken::BeginPlay()
{
	Super::BeginPlay();
	
    //Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called every frame
void APlayerChicken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChicken::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        //Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerChicken::Look);

        //Flap L
        EnhancedInputComponent->BindAction(FlapLAction, ETriggerEvent::Started, this, &APlayerChicken::FlapL);

        //Flap R
        EnhancedInputComponent->BindAction(FlapRAction, ETriggerEvent::Started, this, &APlayerChicken::FlapR);

        //Set Checkpoint
        EnhancedInputComponent->BindAction(CheckpointAction, ETriggerEvent::Started, this, &APlayerChicken::StartCheckpoint);
        EnhancedInputComponent->BindAction(CheckpointAction, ETriggerEvent::Completed, this, &APlayerChicken::StopCheckpoint);

        //Respawn
        EnhancedInputComponent->BindAction(RespawnAction, ETriggerEvent::Started, this, &APlayerChicken::Respawn);
    }
}


void APlayerChicken::FlapL()
{
    FVector Forward = GetActorForwardVector(); Forward.Z = 0.0f; Forward.Normalize();
    SphereComponent->AddImpulseAtLocation(GetTransform().TransformVector(FlapImpulseAtLocation), LeftWingComponent->GetComponentLocation());
    SphereComponent->AddImpulse(GetActorUpVector() * JumpFlapImpulse);
    SphereComponent->AddImpulse(Forward * ForwardFlapImpulse);
    UGameplayStatics::PlaySound2D(GetWorld(), CluckLightSound);
    Mesh->GetAnimInstance()->Montage_Play(FlapLMontage);
}

void APlayerChicken::FlapR()
{
    FVector FlapRImpulseAtLocation = FlapImpulseAtLocation; FlapRImpulseAtLocation.Y = -FlapRImpulseAtLocation.Y;
    FVector FlapRTorque = FlapTorque; FlapTorque.Z = -FlapTorque.Z;
    FVector Forward = GetActorForwardVector(); Forward.Z = 0.0f; Forward.Normalize();

    SphereComponent->AddImpulseAtLocation(GetTransform().TransformVector(FlapRImpulseAtLocation), RightWingComponent->GetComponentLocation());
    SphereComponent->AddImpulse(GetActorUpVector() * JumpFlapImpulse);
    SphereComponent->AddImpulse(Forward * ForwardFlapImpulse);
    UGameplayStatics::PlaySound2D(GetWorld(), CluckLightSound);
    Mesh->GetAnimInstance()->Montage_Play(FlapRMontage);
}

void APlayerChicken::Respawn()
{
    //Get Game Mode
    AChikenRageGameGameMode* gamemode = Cast<AChikenRageGameGameMode>(GetWorld()->GetAuthGameMode());
    if (!IsValid(gamemode)) return;

    //Ensure that the current checkpoint is set
    if (!IsValid(gamemode->CurrentCheckpoint)) return;

    //Respawn
    UGameplayStatics::PlaySound2D(GetWorld(), ClucHeavySound);
    FOutputDeviceNull ar;
    const FString command = FString::Printf(TEXT("RespawnPlayer"));
    gamemode->CurrentCheckpoint->CallFunctionByNameWithArguments(*command, ar, NULL, true);
}

void APlayerChicken::StartCheckpoint()
{
    //Get Game Mode
    AChikenRageGameGameMode* gamemode = Cast<AChikenRageGameGameMode>(GetWorld()->GetAuthGameMode());
    if (!IsValid(gamemode)) return;

    //Spawn checkpoint spawner if the player have eggs remaining
    if (gamemode->CheckpointNum <= 0) return;

    //Spawn checkpoint spawner if one does not currently exist
    if (IsValid(CurrentCheckpointSpawner)) return;

    //Spawn checkpoint spawner after a duration of the input being held
    GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &APlayerChicken::Checkpoint, SpawnCheckpointDuration, false);
}

void APlayerChicken::Checkpoint()
{
    //Spawn Checkpoint and launch player up
    CurrentCheckpointSpawner = GetWorld()->SpawnActor<AActor>(CheckpointSpawner, GetActorTransform());
    SphereComponent->AddImpulse(GetActorUpVector() * JumpImpulseOnCheckpoint);
    UGameplayStatics::PlaySound2D(GetWorld(), ClucHeavySound);

    //Update Game Mode
    AChikenRageGameGameMode* gamemode = Cast<AChikenRageGameGameMode>(GetWorld()->GetAuthGameMode());
    if (IsValid(gamemode)) gamemode->CheckpointNum--;
}

void APlayerChicken::StopCheckpoint()
{
    //Cancel checkpoint spawning upon releasing input
    GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
}

void APlayerChicken::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}
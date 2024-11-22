// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerChicken.generated.h"

UCLASS()
class CHIKENRAGEGAME_API APlayerChicken : public APawn
{
	GENERATED_BODY()

#pragma region Events
public:
	// Sets default values for this pawn's properties
	APlayerChicken();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Components
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	/** The SphereComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereComponent;

	/** Left wing impulse point */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* LeftWingComponent;

	/** Right wing impulse point */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RightWingComponent;

#pragma endregion

#pragma region Input
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Flap L Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FlapLAction;

	/*Called for flapping the left wing*/
	void FlapL();

	/** Flap R Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FlapRAction;

	/*Called for flapping the right wing*/
	void FlapR();

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Checkpoint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CheckpointAction;

	/** Respawn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RespawnAction;

	/*Called for flapping respawning*/
	void Respawn();

	/*Called for start spawning checkpoint*/
	void StartCheckpoint();
	/*Called for spawning checkpoint*/
	void Checkpoint();
	/*Called for stop spawning checkpoint*/
	void StopCheckpoint();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

#pragma endregion

#pragma region Animation
private:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FlapLMontage;

	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FlapRMontage;

#pragma endregion
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoundBase* CluckLightSound;

	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoundBase* ClucHeavySound;

#pragma region Audio
private:

#pragma endregion

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		FVector FlapImpulseAtLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float JumpFlapImpulse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float ForwardFlapImpulse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		FVector FlapTorque;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float UprightTorqueStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> CheckpointSpawner;
		FTimerHandle ShootingTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		AActor* CurrentCheckpointSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float SpawnCheckpointDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float JumpImpulseOnCheckpoint;
};

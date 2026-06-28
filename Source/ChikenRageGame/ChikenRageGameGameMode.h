// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChikenRageGameGameMode.generated.h"

UCLASS(minimalapi)
class AChikenRageGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Constructors, Destructors, & Events
public:
	AChikenRageGameGameMode();
protected:
	virtual void Tick(float DeltaTime) override;

#pragma endregion

public:
	/* Accumulated time spent to complete the level **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double Time;

	/** Total number of checkpoints available to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoints", meta = (AllowPrivateAccess = "true"))
	int32 CheckpointNum = 6;

	/* Reference to the checkpoint actor the player is currently using **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoints", meta = (AllowPrivateAccess = "true"))
	AActor* CurrentCheckpoint;

	/* Completes level **/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CompleteLevel();
	virtual void CompleteLevel_Implementation() {};

};
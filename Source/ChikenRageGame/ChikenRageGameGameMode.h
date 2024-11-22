// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChikenRageGameGameMode.generated.h"

UCLASS(minimalapi)
class AChikenRageGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChikenRageGameGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	double Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int CheckpointNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentCheckpoint;
};
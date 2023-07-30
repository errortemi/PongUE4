// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Ball.h"
#include "Paddle.h"
#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APongGameModeBase();
	virtual void PostLogin(APlayerController* newPlayer) override;
	void AttachActors();

private:
	void ShowMessage();
	UPROPERTY(EditAnywhere)
		float WaitTime{ 1 };

	FTimerHandle Timer;
	int PlayersCount{0};
	TArray<class APaddle*> Players;
	ACameraActor* Camera;
	ABall* TheBall;
	bool IsGameStarted{false};
};

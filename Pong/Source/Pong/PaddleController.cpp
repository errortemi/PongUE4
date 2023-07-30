// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleController.h"
#include "Paddle.h"

void APaddleController::SetupInputComponent()
{
    Super::SetupInputComponent();
}

void APaddleController::MovePaddle(float Value)
{
    APaddle* Paddle = Cast<APaddle>(GetPawn());
    FVector ScreenPosition = GetPawn()->GetActorLocation();

    if (Paddle)
        Paddle->MovePaddle(Value);
}

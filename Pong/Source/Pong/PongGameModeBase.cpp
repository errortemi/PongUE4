// Copyright Epic Games, Inc. All Rights Reserved.


#include "PongGameModeBase.h"
#include "EngineUtils.h"

APongGameModeBase::APongGameModeBase()
{
    Players.Init(nullptr, 2);
}

void APongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    GetWorldTimerManager().SetTimer(Timer, this, &APongGameModeBase::ShowMessage, WaitTime, false);

    if (!Players[0]|| !Players[1])
        AttachActors();

    NewPlayer->Possess(Players[PlayersCount++]);
    NewPlayer->SetViewTargetWithBlend(Camera);

    if (PlayersCount >= 2)
        TheBall->StartMoving();
}

void APongGameModeBase::AttachActors()
{
    for (TActorIterator<APaddle> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName() == "LeftPlayer")
            Players[0] = *ActorItr;
        else if (ActorItr->GetName() == "RightPlayer")
            Players[1] = *ActorItr;
    }
    for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName() == "Camera")
            Camera = *ActorItr;
    }
    for (TActorIterator<ABall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName() == "Ball_1")
            TheBall = *ActorItr;
    }
}

void APongGameModeBase::ShowMessage()
{
    if (PlayersCount < 2)
    {
        FString message("waiting for player 2");
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s"), *message));
    }
}

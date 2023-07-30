// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class PONG_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

public:
	UFUNCTION(Reliable, Server)
		void StartMoving();
	void StartMoving_Implementation();

private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplyVelocityOnRespawn();


private:

	UPROPERTY(EditAnywhere)
		float WaitTime{1};

	FTimerHandle Timer;
	float MovementSpeed = 1000.f;

	FVector MovementDirectionAfterSpawn{0,0,0};

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;
};

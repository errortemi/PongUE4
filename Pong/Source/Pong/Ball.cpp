// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Gate.h"
#include "Paddle.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = Mesh;

	SetReplicateMovement(true);
	SetReplicates(true);

	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(false);
	Mesh->SetConstraintMode(EDOFMode::XYPlane);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	ProjectileMovement->bBounceAngleAffectsFriction = false;
}

void ABall::StartMoving_Implementation()
{
	bool RandomInitialDirection = UKismetMathLibrary::RandomBool();
	MovementDirectionAfterSpawn = FVector(RandomInitialDirection ? 1.0 : -1.0, 0.0, 0.0);
	GetWorldTimerManager().SetTimer(Timer, this, &ABall::ApplyVelocityOnRespawn, WaitTime, false);
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (dynamic_cast<APaddle*>(OtherActor))
	{
		FVector BounceAngle = GetActorLocation() - OtherActor->GetActorLocation();
		BounceAngle.Normalize();
		Mesh->SetPhysicsLinearVelocity(BounceAngle * MovementSpeed);
	}
	else if (dynamic_cast<AGate*>(OtherActor))
	{
		FString message("Goal");
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Green, FString::Printf(TEXT("%s!"), *message));
		Mesh->SetAllPhysicsPosition(FVector{ 0, 0, 60 });
		Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Mesh->SetPhysicsAngularVelocity(FVector::ZeroVector);
		StartMoving();
	}
}

void ABall::ApplyVelocityOnRespawn()
{
	Mesh->SetPhysicsLinearVelocity(MovementDirectionAfterSpawn * MovementSpeed);
}

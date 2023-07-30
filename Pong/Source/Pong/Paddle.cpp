// Fill out your copyright notice in the Description page of Project Settings.

#include "Paddle.h"
#include "Net/UnrealNetwork.h"
// Sets default values
APaddle::APaddle()
{
	PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
    RootComponent = Mesh;

    bAlwaysRelevant = true;
    SetReplicates(true);
    SetReplicateMovement(true);
    Mesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    const FVector MoveDirection = ConsumeMovementInputVector();

    // Calculate  movement based on speed and direction
    const FVector Movement = MoveDirection * MovementSpeed * DeltaTime;

    // If non-zero size, move this actor
    if (Movement.SizeSquared() > 0.0f)
    {
        FHitResult HitResult(1.f);
        Mesh->MoveComponent(Movement, FRotator::ZeroRotator, true, &HitResult);

        if (HitResult.IsValidBlockingHit())
        {
            const FVector Normal2D = HitResult.Normal.GetSafeNormal2D();
            const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - HitResult.Time);
            Mesh->MoveComponent(Deflection, FRotator::ZeroRotator, true);
        }
    }
}

void APaddle::Input(float Value)
{
    if (GetRemoteRole() != ROLE_Authority)
    {
        MovePaddle(Value);
        MovePaddle_Implementation(Value);
    }
    else
        MovePaddle(Value);
}

// Called to bind functionality to input
void APaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveHorizontal", this, &APaddle::Input);
    }
}


void APaddle::MovePaddle_Implementation(float Value)
{
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "UEFPSLevelUp/Player/Public/ShooterCharacter.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}
	if(ShooterCharacter)
	{
		//获取速度
		FVector Velocity{ShooterCharacter -> GetVelocity()};
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = ShooterCharacter -> GetCharacterMovement() -> IsFalling();

		if(ShooterCharacter -> GetCharacterMovement() -> GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

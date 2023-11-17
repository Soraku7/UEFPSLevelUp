// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

		//玩家控制器方向
		FRotator AnimRotation = ShooterCharacter -> GetBaseAimRotation();
		FString RotationMessage = FString::Printf(TEXT("Base Anim Rotation: %f") , MovementOffset);

		//获取移动方向与x轴的旋转角
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter -> GetVelocity());
		MovementOffset = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation , AnimRotation).Yaw;

		if(ShooterCharacter -> GetVelocity().Size() > 0.f)
			LastMovementOffsetYaw = MovementOffset;

		
		if(GEngine)
		{
			GEngine -> AddOnScreenDebugMessage(1 , 0.f , FColor::White , RotationMessage);
		}
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

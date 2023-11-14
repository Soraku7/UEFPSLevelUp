// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USoundCue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
UCLASS()
class UEFPSLEVELUP_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const{return CameraBoom;}

	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	
	/**
	 * 左右旋转
	 * @param Rate 旋转速度
	 */
	void TurnAtRate(float Rate);

	/**
	 * 上下旋转
	 * @param Rate 旋转速度
	 */
	void LookUpAtRate(float Rate);

	void FireWeapon();

	
private:
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat , meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat , meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat , meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontage;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat , meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat , meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;
	
};

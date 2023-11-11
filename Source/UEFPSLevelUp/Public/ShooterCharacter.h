// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

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
	
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
private:
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "True"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "True"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "True"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "True"))
	float BaseLookUpRate;
};

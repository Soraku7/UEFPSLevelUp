// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AWeapon;
class AItem;
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

	void AimingButtonPressed();
	void AimingButtonReleased();

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

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation , FVector& OutBeamLocation);

	void CameraInterpZoom(float DeltaTime);

	//设置旋转速度
	void SetLookRate();

	void Turn(float Rate);

	void LookUp(float Rate);

	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();
	
	UFUNCTION()
	void FinishCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	
	/**
	 * 射线检索拾取物体
	 */
	bool TraceUnderCrosshair(FHitResult& OutHitResult , FVector& OutHitLocation);

	
	/**
	 * 当重叠Item>0时
	 */
	void  TraceForItems();

	AWeapon* SpawnDefaultWeapon();

	
	/**
	 * 装备武器
	 */
	void EquipWeapon(AWeapon* WeaponToEquip);
	
	/**
	 * 丢落武器
	 */
	void DropWeapon();

	void SelectButtonPressed();
	void SelectButtonReleased();

	/**
	 * 丢弃当前武器并替换
	 * @param WeaponToSwap 
	 */
	void SwapWeapon(AWeapon* WeaponToSwap);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const{return CameraBoom;}

	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}

	FORCEINLINE bool GetAiming() const {return bAiming;}

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;
	
	FORCEINLINE int8 GetOverlappedItemCount() const {return OverlappedItemCount;}

	
	/**
	 * 增加或者减少重叠Item数量
	 * @param Amount 增加或减少数量
	 */
	void IncrementOverlappedItemCount(int8 Amount);
	

private:
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true") , meta = (ClampMin = "0.0" , ClampMax = "1.0" , UIMin = "0.0" , UIMax = "1.0"))
	float HipTurnRate;
	
	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true") , meta = (ClampMin = "0.0" , ClampMax = "1.0" , UIMin = "0.0" , UIMax = "1.0"))
	float HipLookUpRate;
	
	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true") , meta = (ClampMin = "0.0" , ClampMax = "1.0" , UIMin = "0.0" , UIMax = "1.0"))
	float AimingTurnRate;

	UPROPERTY(VisibleAnywhere ,	BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true") , meta = (ClampMin = "0.0" , ClampMax = "1.0" , UIMin = "0.0" , UIMax = "1.0"))
	float AimingLookUpRate;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float MouseHipTurnRate;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float MouseHipLookUpRate;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float MouseAimingTurnRate;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	float MouseAimingLookUpRate;
	
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

	/**
	 * 是否瞄准
	 */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly ,	Category = combat , meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	/**
	 * 相机常规视野值
	 */
	float CameraDefaultFOV;

	/**
	 * 放大后的相机视野值
	 */
	float CameraZoomedFOV;

	float CameraCurrentFOV;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = combat , meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Crosshair , meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Crosshair , meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Crosshair , meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Crosshair , meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Crosshair , meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;
	
	bool bFireButtonPress;

	/**
	 * 是否可以进行射击
	 */
	bool bShouldFire;

	float AutomaticFireRate;

	FTimerHandle AutoFireTimer;
	/**
	 * 是否追踪物体
	 */
	bool bShouldTraceForItems;

	int8 OverlappedItemCount;

	
	/**
	 * 最后一帧选中的物体
	 */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Items , meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItemLastFrame;

	/**
	 * 装备的武器
	 */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Combat , meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	
	/**
	 * 武器蓝图
	 */
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = Combat , meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/**
	 * 追踪选中物体
	 */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Combat , meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;
};

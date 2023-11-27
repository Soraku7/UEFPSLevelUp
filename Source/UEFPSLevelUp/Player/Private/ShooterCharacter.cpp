// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFPSLevelUp/Player/Public/ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "UEFPSLevelUp/Item/Public/Item.h"


// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	bAiming(false),
//相机
	CameraDefaultFOV(0.f),
	CameraZoomedFOV(60.f),
	CameraCurrentFOV(0.f),
	ZoomInterpSpeed(20.f),
//开镜关镜上下左右移动速度
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
//开镜关镜鼠标移动速度
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(1.0f),
	MouseAimingLookUpRate(1.0f),
//瞄准分散因素
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelocityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairAimFactor(0.f),
	CrosshairShootingFactor(0.f),
//子弹射击时间变量
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
//枪的射击间隔
	AutomaticFireRate(0.1f),
	bShouldFire(true),
	bFireButtonPress(false),
//追踪物体
	bShouldTraceForItems(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom -> SetupAttachment(RootComponent);
	CameraBoom -> TargetArmLength = 180.0f;
	CameraBoom -> bUsePawnControlRotation = true;
	CameraBoom -> SocketOffset = FVector(0.f , 50.f , 70.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera -> SetupAttachment(CameraBoom , USpringArmComponent::SocketName);
	FollowCamera -> bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = false;

	//玩家沿加速度方向旋转
	GetCharacterMovement() -> bOrientRotationToMovement = false;
	GetCharacterMovement() -> RotationRate = FRotator(0.f , 540.0f , 0.f);

	GetCharacterMovement() -> JumpZVelocity = 600.f;
	//角色掉落时的左右速度控制能力
	GetCharacterMovement() -> AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera() -> FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
}

void AShooterCharacter::MoveForward(float value)
{
	if((Controller != nullptr) && (value != 0))
	{
	    const FRotator Rotation = Controller -> GetControlRotation();
		const FRotator YawRotation(0 , Rotation.Yaw , 0);

		//获得Yaw的旋转矩阵 并且取得正向轴
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction , value);
	}
}

void AShooterCharacter::MoveRight(float value)
{
	if((Controller != nullptr) && (value != 0))
	{	
		const FRotator Rotation = Controller -> GetControlRotation();
		const FRotator YawRotation(0 , Rotation.Yaw , 0);

		//获得Yaw的旋转矩阵 并且取得正向轴
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction , value);
	}
}

void AShooterCharacter::AimingButtonPressed()
{
	bAiming = true;
	GetFollowCamera() -> SetFieldOfView(CameraZoomedFOV);
}

void AShooterCharacter::AimingButtonReleased()
{
	bAiming = false;
	GetFollowCamera() -> SetFieldOfView(CameraDefaultFOV);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraInterpZoom(DeltaTime);
	SetLookRate();

	CalculateCrosshairSpread(DeltaTime);

	TraceForItems();

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//检测玩家输入组件
	check(PlayerInputComponent);

	PlayerInputComponent -> BindAxis("MoveForward" , this , &AShooterCharacter::MoveForward);
	PlayerInputComponent -> BindAxis("MoveRight" , this , &AShooterCharacter::MoveRight);
	PlayerInputComponent -> BindAxis("TurnRate" , this , &AShooterCharacter::TurnAtRate);
	PlayerInputComponent -> BindAxis("LookUpRate" , this , &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent -> BindAxis("Turn" , this , &AShooterCharacter::Turn);
	PlayerInputComponent -> BindAxis("LookUp" , this , &AShooterCharacter::LookUp);

	PlayerInputComponent -> BindAction("Jump" , IE_Pressed , this , & ACharacter::Jump);
	PlayerInputComponent -> BindAction("Jump" , IE_Released , this , & ACharacter::StopJumping);

	PlayerInputComponent -> BindAction("FireButton" , IE_Pressed , this , &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent -> BindAction("FireButton" , IE_Released , this , &AShooterCharacter::FireButtonReleased);


	PlayerInputComponent -> BindAction("AimingButton" , IE_Pressed , this , &AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent -> BindAction("AimingButton" , IE_Released , this , &AShooterCharacter::AimingButtonReleased);

}

float AShooterCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if(OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld() -> GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld() -> GetDeltaSeconds());
}

void AShooterCharacter::FireWeapon()
{
	if(FireSound)
	{
		UGameplayStatics::PlaySound2D(this , FireSound);
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh() -> GetSocketByName("BarrelSocket");
	if(BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket -> GetSocketTransform(GetMesh());

		if(MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , MuzzleFlash , SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation()
			, BeamEnd);

		if(bBeamEnd)
		{
			if(ImpactParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , ImpactParticle , BeamEnd);
			}
			
			if(BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , BeamParticles , SocketTransform);
				if(Beam)
				{
					Beam -> SetVectorParameter(FName("Target") , BeamEnd);
				}
			}
		}
		
		
	}

	UAnimInstance* AnimInstance = GetMesh() -> GetAnimInstance();
	if(AnimInstance && HipFireMontage)
	{
		AnimInstance -> Montage_Play(HipFireMontage);
		AnimInstance -> Montage_JumpToSection(FName("StartFire"));
	}

	StartCrosshairBulletFire();
}

bool AShooterCharacter::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	//屏幕中线发出射线
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshair(CrosshairHitResult , OutBeamLocation);

	if(bCrosshairHit)
	{
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else
	{
		//屏幕中心未能检测到目标
	}
	//枪口发出射线
	FHitResult WeaponTraceHit;
	const FVector WeaponTraceStart{MuzzleSocketLocation};
	const FVector StartToEnd{OutBeamLocation - MuzzleSocketLocation};
	const FVector WeaponTraceEnd{OutBeamLocation + StartToEnd * 1.25f};
	GetWorld() -> LineTraceSingleByChannel(WeaponTraceHit , WeaponTraceStart , WeaponTraceEnd , ECC_Visibility);
	if(WeaponTraceHit.bBlockingHit)
	{
		OutBeamLocation = WeaponTraceHit.Location;
		return true;
	}
	return false;
	
	
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	if(bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV , CameraZoomedFOV , DeltaTime , ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV , CameraDefaultFOV , DeltaTime , ZoomInterpSpeed);
	}
	GetFollowCamera() -> SetFieldOfView(CameraCurrentFOV);
}

void AShooterCharacter::SetLookRate()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}

}

void AShooterCharacter::Turn(float Rate)
{
	float TurnScaleFactor;
	if(bAiming)
		TurnScaleFactor = MouseAimingTurnRate;
	else
		TurnScaleFactor = MouseHipTurnRate;

	AddControllerYawInput(Rate * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Rate)
{
	float TurnScaleFactor;
	if(bAiming)
		TurnScaleFactor = MouseAimingLookUpRate;
	else
		TurnScaleFactor = MouseHipLookUpRate;

	AddControllerPitchInput(Rate * TurnScaleFactor);
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{0.0 , 600.f};
	FVector2D VelocityMultiplierRange{0.f , 1.f};
	FVector Velocity{GetVelocity()};
	Velocity.Z = 0;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange
		, VelocityMultiplierRange , Velocity.Size());

	
	//计算瞄准误差
	if(GetCharacterMovement() -> IsFalling())
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor
			, 2.25f, DeltaTime , 2.25f);
	else
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor
			, 0.f, DeltaTime , 30.f);

	if(bAiming)
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor
			, 0.6f , DeltaTime , 30.f);
	else
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor
			, 0.f, DeltaTime , 30.f);

	if(bFiringBullet)
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor
			, 0.3f , DeltaTime , 60.f);
	else
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor
			, 0.f , DeltaTime , 60.f);
	
	CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;

}

void AShooterCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;

	GetWorldTimerManager().SetTimer(CrosshairShootTimer , this , &AShooterCharacter::FinishCrosshairBulletFire , ShootTimeDuration);
}

void AShooterCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPress = true;
	StartFireTimer();
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPress = false;
}

void AShooterCharacter::StartFireTimer()
{
	if(bShouldFire)
	{
		FireWeapon();
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(AutoFireTimer , this 
			, &AShooterCharacter::AutoFireReset , AutomaticFireRate);
	}
}

void AShooterCharacter::AutoFireReset()
{
	bShouldFire = true;
	if(bFireButtonPress)
	{
		StartFireTimer();
	}
}

bool AShooterCharacter::TraceUnderCrosshair(FHitResult& OutHitResult , FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine -> GameViewport)
	{
		GEngine -> GameViewport -> GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f , ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this , 0),
		CrosshairLocation , CrosshairWorldPosition , CrosshairWorldDirection);

	if(bScreenToWorld)
	{
		const FVector Start{CrosshairWorldPosition};
		const FVector End{Start + CrosshairWorldDirection * 50000.f};
		OutHitLocation = End;
		GetWorld() -> LineTraceSingleByChannel(OutHitResult , Start , End , ECC_Visibility);
		if(OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void AShooterCharacter::TraceForItems()
{
	if(bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		TraceUnderCrosshair(ItemTraceResult , HitLocation);
		if(ItemTraceResult.bBlockingHit)
		{
			AItem* HitItem = Cast<AItem>(ItemTraceResult.GetActor());
			if(HitItem && HitItem -> GetPickupWidget())
			{
				HitItem -> GetPickupWidget() -> SetVisibility(true);
			}
		}
	}
	
}

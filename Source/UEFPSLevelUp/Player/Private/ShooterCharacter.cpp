// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFPSLevelUp/Player/Public/ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom -> SetupAttachment(RootComponent);
	CameraBoom -> TargetArmLength = 300.0f;
	CameraBoom -> bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera -> SetupAttachment(CameraBoom , USpringArmComponent::SocketName);
	FollowCamera -> bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//玩家沿加速度方向旋转
	GetCharacterMovement() -> bOrientRotationToMovement = true;
	GetCharacterMovement() -> RotationRate = FRotator(0.f , 540.0f , 0.f);

	GetCharacterMovement() -> JumpZVelocity = 600.f;
	//角色掉落时的左右速度控制能力
	GetCharacterMovement() -> AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
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

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayerInputComponent -> BindAxis("Turn" , this , &APawn::AddControllerYawInput);
	PlayerInputComponent -> BindAxis("LookUp" , this , &APawn::AddControllerPitchInput);

	PlayerInputComponent -> BindAction("Jump" , IE_Pressed , this , & ACharacter::Jump);	PlayerInputComponent -> BindAction("Jump" , IE_Pressed , this , & ACharacter::Jump);
	PlayerInputComponent -> BindAction("Jump" , IE_Released , this , & ACharacter::StopJumping);

	PlayerInputComponent -> BindAction("FireButton" , IE_Pressed , this , &AShooterCharacter::FireWeapon);
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
	}
}


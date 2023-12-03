// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFPSLevelUp/Item/Public/Weapon.h"


// Sets default values
AWeapon::AWeapon():
	ThrowWeaponTime(0.7f),
	bFalling(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{0.f , GetItemMesh() -> GetComponentRotation().Yaw , 0.f};
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
		GetItemMesh() -> SetWorldRotation(MeshRotation);
	}
}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{0.f , GetItemMesh() -> GetComponentRotation().Yaw , 0.f};
	GetItemMesh() -> SetWorldRotation(MeshRotation , false , nullptr , ETeleportType::TeleportPhysics);

	const FVector MeshForward{GetItemMesh() -> GetForwardVector()};
	const FVector MeshRight{GetItemMesh() -> GetRightVector()};
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f , MeshForward);

	float RandomRotation = FMath::FRandRange(0.f , 30.f);
	ImpulseDirection = ImpulseDirection.RotateAngleAxisRad(RandomRotation , FVector(0.f , 0.f , 1.f));
	ImpulseDirection *= 2000.f;
	GetItemMesh() -> AddImpulse(ImpulseDirection);

	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer , this , &AWeapon::StopFalling, ThrowWeaponTime);
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
}


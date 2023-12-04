﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFPSLevelUp/Item/Public/Item.h"

#include "VectorUtil.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UEFPSLevelUp/Player/Public/ShooterCharacter.h"


// Sets default values
AItem::AItem():
	ItemName(FString("Default")),
	ItemCount(0),
	ItemRarity(EItemRarity::EIR_Common),
	ItemState(EItemState::EIS_Pickup),
//物体线性插值
	ZCurveTime(0.7f),
	ItemInterpStartLocation(FVector(0.f)),
	CameraTargetLocation(FVector(0.f)),
	bInterping(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox -> SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox -> SetCollisionResponseToChannel(ECC_Visibility , ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget -> SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere -> SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	SetActiveStars();

	AreaSphere -> OnComponentBeginOverlap.AddDynamic(this , &AItem::OnSphereOverlap);
	AreaSphere -> OnComponentEndOverlap.AddDynamic(this , &AItem::AItem::OnSphereEndOverlap);

	SetItemProperties(ItemState);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if(ShooterCharacter)
		{
			ShooterCharacter -> IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if(ShooterCharacter)
		{
			ShooterCharacter -> IncrementOverlappedItemCount(-1);
		}
	}
}

void AItem::SetActiveStars()
{
	for(int32 i = 0 ; i <= 5 ; i ++)
	{
		ActiveStars.Add(false);
	}
	switch (ItemRarity)
	{
	case EItemRarity::EIR_Damaged:
		ActiveStars[1] = true;
		break;
	case EItemRarity::EIR_Common:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		break;
	case EItemRarity::EIR_UnCommon:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		break;
	case EItemRarity::EIR_Rare:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		break;
	case EItemRarity::EIR_Legendary:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		ActiveStars[5] = true;
		break;
	}
}

void AItem::SetItemProperties(EItemState State)
{
	switch(State)
	{
	case EItemState::EIS_Pickup:
		ItemMesh -> SetSimulatePhysics(false);
		ItemMesh -> SetEnableGravity(false);
		ItemMesh -> SetVisibility(true);
		ItemMesh -> SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere -> SetCollisionResponseToAllChannels(ECR_Overlap);
		AreaSphere -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		CollisionBox -> SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionBox -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility , ECR_Block);
		CollisionBox -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EItemState::EIS_Equipped:
		PickupWidget -> SetVisibility(false);
		ItemMesh -> SetSimulatePhysics(false);
		ItemMesh -> SetEnableGravity(false);
		ItemMesh -> SetVisibility(true);
		ItemMesh -> SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere -> SetCollisionResponseToAllChannels(ECR_Ignore);
		AreaSphere -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		CollisionBox -> SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionBox -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility , ECR_Block);
		CollisionBox -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	case EItemState::EIS_Falling:
		ItemMesh -> SetSimulatePhysics(true);
		ItemMesh -> SetEnableGravity(true);
		ItemMesh -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh -> SetCollisionResponseToChannels(ECR_Ignore);
		ItemMesh -> SetCollisionResponseToChannel(ECC_WorldStatic , ECR_Block);
		ItemMesh -> SetCollisionResponseToChannel(ECC_WorldStatic , ECR_Block);
		ItemMesh -> SetCollisionResponseToChannel(ECC_WorldDynamic , ECR_Block);
		
		AreaSphere -> SetCollisionResponseToAllChannels(ECR_Ignore);
		AreaSphere -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		CollisionBox -> SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionBox -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	}
}

void AItem::FinishInterping()
{
	if(Character)
	{
		Character -> GetPickupItem(this);
	}
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

void AItem::StartItemCurve(AShooterCharacter* Char)
{
	Character = Char;

	ItemInterpStartLocation = GetActorLocation();
	bInterping = true;
	SetItemState(EItemState::EIS_EquipInterping);

	GetWorldTimerManager().SetTimer(ItemInterpTimer , this , &AItem::FinishInterping , ZCurveTime);
}


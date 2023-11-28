﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UEFPSLevelUp/Item/Public/Item.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UEFPSLevelUp/Player/Public/ShooterCharacter.h"


// Sets default values
AItem::AItem():
	ItemName(FString("Default")),
	ItemCount(0),
	ItemRarity(EItemRarity::EIR_Common)
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


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

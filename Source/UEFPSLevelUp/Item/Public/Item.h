// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class UEFPSLEVELUP_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,   
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,   
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Item Properties" , meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Item Properties" , meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	/**
	 * 拾取物品信息UI
	 */
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Item Properties" , meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PickupWidget;

	/**
	 * 物品与人物重叠进行信息显示
	 */
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Item Properties" , meta = (AllowPrivateAccess = "true"))
	USphereComponent* AreaSphere;
public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const {return PickupWidget;};

};

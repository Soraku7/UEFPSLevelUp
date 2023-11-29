// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_UnCommon UMETA(DisplayName = "UnCommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(Display = "DefaultMAX")

};

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

	
	/**
	 * 设置物品稀有度
	 */
	void SetActiveStars();
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

	/**
	 * 物品名字
	 */
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Item Propertier" , meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	
	/**
	 * 物体数量
	 */
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Item Propertier" , meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	
	/**
	 * 物品稀有度
	 */
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Item Propertier" , meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Item Propertier" , meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;
	
public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const {return PickupWidget;}
	
	FORCEINLINE USphereComponent* GetAreaSphere() const {return AreaSphere;}

	FORCEINLINE UBoxComponent* GetCollisionBox() const {return CollisionBox;}
};

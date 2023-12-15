// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class SURVIVALGAME_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	ALadder();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;



	void ConvertItem();

	void ConvertStructure();


/*---------------------------- 컴포넌트 -----------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* LadderStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* PickUpCollision;




/*---------------------------- 저장 정보 -----------------------------------------*/






/*---------------------------- 변수 -----------------------------------------*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HaveToPickUp = false;

};

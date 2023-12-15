

#include "Ladder.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/OutputDeviceNull.h"

ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = true;

	LadderStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// �÷��̾� ���̷�Ż �޽�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Script/Engine.StaticMesh'/Game/Scource/Ladder/medieval_wooden-ladder.medieval_wooden-ladder'"));
	if (SM.Succeeded()) {
		LadderStaticMesh->SetStaticMesh(SM.Object);
	}
	
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();

	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("TestFunc"), ar, NULL, true);
	
}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALadder::ConvertItem() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ConvertItem"), ar, NULL, true);
}

void ALadder::ConvertStructure() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ConvertStructure"), ar, NULL, true);
}


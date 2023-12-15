// Copyright Epic Games, Inc. All Rights Reserved.


#include "SurvivalGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer_HUD_Widget.h"
#include "MyPlayer.h"

ASurvivalGameGameModeBase::ASurvivalGameGameModeBase() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/BP_MyPlayer.BP_MyPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	static ConstructorHelpers::FClassFinder<UMyPlayer_HUD_Widget> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/BP_HUD.BP_HUD_C'"));
	if (UI_HUD.Succeeded()) {
		HUD_Class = UI_HUD.Class;

		CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (CurrentWidget) {
			//CurrentWidget->AddToViewport();
		}
	}


}

void ASurvivalGameGameModeBase::BeginPlay() {

	UWorld* World = GetWorld();
	if (World) {
		MyCharacter = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}

}

void ASurvivalGameGameModeBase::AddToViewport_HP() {

	if (CurrentWidget) {
		CurrentWidget->AddToViewport();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include"Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include"Blueprint/UserWidget.h"
#include"OnlineSubsystem.h"

#include"PlatformTrigger.h"
#include"MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"



UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance COnstructor"));
	
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr)) return;
	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(GameMenuBPClass.Class != nullptr)) return;
	GameMenuClass = GameMenuBPClass.Class;

}

void UPuzzlePlatformsGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Found MainMenu class: %s"), *MainMenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Found InGameMenu class: %s"), *GameMenuClass->GetName());

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found online subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no online subsystem"));
	}
}

void UPuzzlePlatformsGameInstance::Host() {
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World!= nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& IPAdress) {
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Blue,FString::Printf(TEXT("Joining %s"),*IPAdress));
	APlayerController* PlayerControler = GetFirstLocalPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;
	PlayerControler->ClientTravel(IPAdress, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu() {

	APlayerController* PlayerControler = GetFirstLocalPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;
	PlayerControler->ClientTravel("/Game/MenuSystem/MenuLevel", ETravelType::TRAVEL_Absolute);
}

//Executed from MainMenuBP BeginPlay
void UPuzzlePlatformsGameInstance::LoadMenuWidget() {
	if (!ensure(MainMenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (!ensure(Menu != nullptr)) return;


	Menu->Setup();
	Menu->SetMenuInterface(this);
}
void UPuzzlePlatformsGameInstance::LoadInGameMenu(){
	if (!ensure(GameMenuClass != nullptr)) return;
	UMenuWidget* GameMenu = CreateWidget<UMenuWidget>(this, GameMenuClass);
	if (!ensure(GameMenu != nullptr)) return;

	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

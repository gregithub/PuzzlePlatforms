// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include"Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include"Blueprint/UserWidget.h"
#include"PlatformTrigger.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance COnstructor"));
	
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host() {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World!= nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& IPAdress) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Blue,FString::Printf(TEXT("Joining %s"),*IPAdress));
	APlayerController* PlayerControler = GetFirstLocalPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;
	PlayerControler->ClientTravel(IPAdress, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMenu() {
	if (!ensure(MenuClass != nullptr)) return;
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;


	Menu->AddToViewport();
}
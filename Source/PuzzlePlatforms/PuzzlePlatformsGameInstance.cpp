// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include"Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance COnstructor"));


}

void UPuzzlePlatformsGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
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
}

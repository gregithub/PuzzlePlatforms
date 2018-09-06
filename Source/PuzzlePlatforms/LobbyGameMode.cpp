// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include"TimerManager.h"
#include"PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	++CurrentNumberOfPlayers;
	if (CurrentNumberOfPlayers >= 2) {
		GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ALobbyGameMode::StartGame, 5);
	}
}

void ALobbyGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	--CurrentNumberOfPlayers;
}
void ALobbyGameMode::StartGame() {
	auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"



void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	++CurrentNuberOfPlayers;
	if (CurrentNuberOfPlayers > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Number of players: %i"),CurrentNuberOfPlayers);

	}
}

void ALobbyGameMode::Logout(AController* Exiting) {
	--CurrentNuberOfPlayers;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include"Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include"Blueprint/UserWidget.h"
#include"OnlineSessionSettings.h"

#include"PlatformTrigger.h"
#include"MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
		
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			UE_LOG(LogTemp, Warning, TEXT("Found session interface."));
			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UPuzzlePlatformsGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no online subsystem"));
	}
}

void UPuzzlePlatformsGameInstance::Host(FString ServerName) {
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else {
			CreateSession();
		}
	}
}
void UPuzzlePlatformsGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {

		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
			SessionSettings.bIsLANMatch = true;
		}
		else {
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success){
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World!= nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");
}
void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		UE_LOG(LogTemp, Warning, TEXT("Destroy previous OnlineSession success."));
		CreateSession();
	}
}
void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success) {
	if (Success && SessionSearch.IsValid() && Menu!=nullptr) {
	UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete."));

	TArray<FServerData> ServerNames;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *SearchResult.GetSessionIdStr());
		FServerData Data;
		Data.HostUsername = SearchResult.Session.OwningUserName;
		Data.TotalPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.TotalPlayers - SearchResult.Session.NumOpenPublicConnections;
		FString ServerName;
		if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName)) {
			Data.Name = ServerName;
		}
		else {
			Data.Name = "Could not find name.";
		}

		ServerNames.Add(Data);
		}

	Menu->SetServerList(ServerNames);
	}
}
void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid())return;


	FString Address; //out parameter for GetResolvedConnectString
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;

	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Joining %s"), *Address));
	APlayerController* PlayerControler = GetFirstLocalPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;

	PlayerControler->ClientTravel(Address, ETravelType::TRAVEL_Absolute);



}

void UPuzzlePlatformsGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr) {
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
}
void UPuzzlePlatformsGameInstance::StartSession() {
	if (SessionInterface.IsValid()) {
		SessionInterface->StartSession(NAME_GameSession);
	}
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

void UPuzzlePlatformsGameInstance::RefreshServerList() {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Searching for sessions."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

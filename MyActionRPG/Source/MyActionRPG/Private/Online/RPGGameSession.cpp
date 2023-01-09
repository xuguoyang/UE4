// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RPGGameSession.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

ARPGGameSession::ARPGGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ARPGGameSession::OnCreateSessionComplete);

		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ARPGGameSession::OnFindSessionsComplete);

		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ARPGGameSession::OnJoinSessionComplete);
	}
}

void ARPGGameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("OnCreateSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), bWasSuccessful);

	FString MapName;

	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

		FOnlineSessionSettings* SessionSettings = Sessions->GetSessionSettings(InSessionName);
		if (SessionSettings)
		{
			SessionSettings->Get(SETTING_MAPNAME, MapName);
		}
	}

	OnCreatePresenceSessionComplete().Broadcast(InSessionName, bWasSuccessful);

	if (bWasSuccessful && !MapName.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true, FString("listen"));
	}
}

void ARPGGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful);

	IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			UE_LOG(LogOnlineGame, Verbose, TEXT("Num Search Results: %d"), SearchSettings->SearchResults.Num());
			for (int32 SearchIdx = 0; SearchIdx < SearchSettings->SearchResults.Num(); SearchIdx++)
			{
				const FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[SearchIdx];
				DumpSession(&SearchResult.Session);
			}

			OnFindSessionsComplete().Broadcast(bWasSuccessful);
		}
	}
}

void ARPGGameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	bool bWillTravel = false;

	UE_LOG(LogOnlineGame, Verbose, TEXT("OnJoinSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), static_cast<int32>(Result));

	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		}
	}

	OnJoinSessionComplete().Broadcast(Result);

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		InternalTravelToSession();
	}
}

bool ARPGGameSession::HostSession(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence, const FString& MapName, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
	if (!OnlineSub)
	{
		return false;
	}

	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
	{
		OnCreateSessionComplete(NAME_GameSession, false);
		return false;
	}

	FOnlineSessionSettings SessionSettings;
	
	SessionSettings.NumPublicConnections = MaxNumPlayers > 0 ? MaxNumPlayers : 0;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinInProgress = true;

	SessionSettings.bIsLANMatch = bIsLAN;
	SessionSettings.bUsesPresence = bIsPresence;
	SessionSettings.bAllowJoinViaPresence = true;

	SessionSettings.Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);

	OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	return Sessions->CreateSession(*UserId, NAME_GameSession, SessionSettings);
}

void ARPGGameSession::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (!OnlineSub)
	{
		return;
	}

	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
	{
		OnFindSessionsComplete(false);
		return;
	}

	SearchSettings = MakeShareable(new FOnlineSessionSearch);
	SearchSettings->bIsLanQuery = bIsLAN;
	SearchSettings->MaxSearchResults = 100;

	if (bIsPresence)
	{
		SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	}

	TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SearchSettings.ToSharedRef();

	OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	Sessions->FindSessions(*UserId, SearchSettingsRef);
}

bool ARPGGameSession::JoinSession(TSharedPtr<const FUniqueNetId> UserId, int32 SessionIndexInSearchResults)
{
	bool bResult = false;

	if (SessionIndexInSearchResults >= 0 && SessionIndexInSearchResults < SearchSettings->SearchResults.Num())
	{
		bResult = JoinSession(UserId, SearchSettings->SearchResults[SessionIndexInSearchResults]);
	}

	return bResult;
}

bool ARPGGameSession::JoinSession(TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult)
{
	bool bResult = false;

	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bResult = Sessions->JoinSession(*UserId, NAME_GameSession, SearchResult);
		}
	}

	return bResult;
}

const TArray<FOnlineSessionSearchResult>& ARPGGameSession::GetSearchResults() const
{
	return SearchSettings->SearchResults;
}

void ARPGGameSession::InternalTravelToSession()
{
	APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	// travel to session
	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub == nullptr)
	{
		return;
	}

	FString URL;
	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid() || !Sessions->GetResolvedConnectString(NAME_GameSession, URL))
	{
		UE_LOG(LogOnlineGame, Warning, TEXT("Failed to travel to session upon joining it"));
		return;
	}

	PlayerController->ClientTravel(URL, TRAVEL_Absolute);
}
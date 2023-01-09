// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online.h"
#include "GameFramework/GameSession.h"
#include "RPGGameSession.generated.h"

/**
 * 
 */
UCLASS()
class MYACTIONRPG_API ARPGGameSession : public AGameSession
{
	GENERATED_BODY()

	ARPGGameSession(const FObjectInitializer& ObjectInitializer);

protected:

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	TSharedPtr<class FOnlineSessionSearch> SearchSettings;

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	DECLARE_EVENT_TwoParams(ARPGGameSession, FOnCreatePresenceSessionComplete, FName /*SessionName*/, bool /*bWasSuccessful*/);
	FOnCreatePresenceSessionComplete CreatePresenceSessionCompleteEvent;

	DECLARE_EVENT_OneParam(ARPGGameSession, FOnFindSessionsComplete, bool /*bWasSuccessful*/);
	FOnFindSessionsComplete FindSessionsCompleteEvent;

	DECLARE_EVENT_OneParam(ARPGGameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type /*Result*/);
	FOnJoinSessionComplete JoinSessionCompleteEvent;
	
public:

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence, const FString& MapName, int32 MaxNumPlayers);

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, int32 SessionIndexInSearchResults);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& SearchResult);

	const TArray<FOnlineSessionSearchResult>& GetSearchResults() const;

	void InternalTravelToSession();

public:

	FOnCreatePresenceSessionComplete& OnCreatePresenceSessionComplete() { return CreatePresenceSessionCompleteEvent; }
	FOnFindSessionsComplete& OnFindSessionsComplete() { return FindSessionsCompleteEvent; }
	FOnJoinSessionComplete& OnJoinSessionComplete() { return JoinSessionCompleteEvent; }

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};

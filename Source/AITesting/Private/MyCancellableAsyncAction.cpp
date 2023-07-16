// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCancellableAsyncAction.h"

UMyCancellableAsyncAction* UMyCancellableAsyncAction::MyCancellableAsyncAction(const UObject* WorldContext, float DelayTime)
{

	// We must have a valid contextual world for this action, so we don't even make it
	// unless we can resolve the UWorld from WorldContext.
	UWorld* ContextW = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("WorldContext is not valid")))
	{
		return nullptr;
	}

	// Create async action and store function arguments
	UMyCancellableAsyncAction* NewAction = NewObject<UMyCancellableAsyncAction>();
	NewAction->ContextWorld = ContextW;
	NewAction->DelayTime = DelayTime;
	// NewAction->RegisterWithGameInstance(ContextW->GetGameInstance());
	return NewAction;
}


void UMyCancellableAsyncAction::Activate()
{
	// When the async action is ready to activate, set a timer using the world's FTimerManager.
	if (const UWorld* World = GetWorld())
	{
		// The timer manager is a singleton, and GetTimerManger() accessor will always return a valid one.
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.SetTimer(
			OngoingDelay,
			FTimerDelegate::CreateLambda( [WeakThis = TWeakObjectPtr<UMyCancellableAsyncAction>(this)]
			{
				// We're passing "this" as a weak pointer, because there is no guarantee that "this" will
				// exist by the time this lambda callback executes.
				if(WeakThis.IsValid() && WeakThis->IsActive())
				{
					// If everything went well, broadcast OnComplete (fire the On Complete pin), and wrap up.
					WeakThis->OnComplete.Broadcast();
					WeakThis->Cancel();
				}
			} ),
			FMath::Max(DelayTime, 0.001f),
			false);
		
		return;
	}

	// If something failed, we can broadcast OnFail, and then wrap up.
	OnFail.Broadcast();
	Cancel();
}


void UMyCancellableAsyncAction::Cancel()
{
	Super::Cancel();

	if (OngoingDelay.IsValid())
	{
		if (const UWorld* World = GetWorld())
		{
			FTimerManager& TimerManager = World->GetTimerManager();
			TimerManager.ClearTimer(OngoingDelay);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomAsyncTask.h"


UCustomAsyncTask* UCustomAsyncTask::CreateCustomAsyncTask(const UObject* WorldContext)
{
	UCustomAsyncTask* NewAction = NewObject<UCustomAsyncTask>();
	return NewAction;
}


void UCustomAsyncTask::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("CustomTickable Tick"));
	this->OnUpdate.Broadcast();
}

bool UCustomAsyncTask::IsTickable() const
{
	return !IsTemplate(RF_ClassDefaultObject);
}

bool UCustomAsyncTask::IsTickableInEditor() const
{
	return false;
}

bool UCustomAsyncTask::IsTickableWhenPaused() const
{
	return false;
}

TStatId UCustomAsyncTask::GetStatId() const
{
	return TStatId();
}

UWorld* UCustomAsyncTask::GetWorld() const
{
	return GetOuter()->GetWorld();
}



void UCustomAsyncTask::Activate()
{
	UE_LOG(LogTemp, Display, TEXT("UCustomAsyncTask::Activate()"));
	
	// When the async action is ready to activate, set a timer using the world's FTimerManager.
	if (const UWorld* World = GetWorld())
	{
		// // The timer manager is a singleton, and GetTimerManger() accessor will always return a valid one.
		// FTimerManager& TimerManager = World->GetTimerManager();
		// TimerManager.SetTimer(
		// 	OngoingDelay,
		// 	FTimerDelegate::CreateLambda( [WeakThis = TWeakObjectPtr<UCustomAsyncTask>(this)]
		// 	{
		// 		// We're passing "this" as a weak pointer, because there is no guarantee that "this" will
		// 		// exist by the time this lambda callback executes.
		// 		if(WeakThis.IsValid() && WeakThis->IsActive())
		// 		{
		// 			// If everything went well, broadcast OnComplete (fire the On Complete pin), and wrap up.
		// 			WeakThis->OnComplete.Broadcast();
		// 			WeakThis->Cancel();
		// 		}
		// 	} ),
		// 	FMath::Max(DelayTime, 0.001f),
		// 	false);
		
		return;
	}

	// If something failed, we can broadcast OnFail, and then wrap up.
	// OnFail.Broadcast();
	Cancel();
}



void UCustomAsyncTask::Cancel()
{
	Super::Cancel();
	
	UE_LOG(LogTemp, Display, TEXT("UCustomAsyncTask::Cancel()"));

	// if (OngoingDelay.IsValid())
	// {
	// 	if (const UWorld* World = GetWorld())
	// 	{
	// 		FTimerManager& TimerManager = World->GetTimerManager();
	// 		TimerManager.ClearTimer(OngoingDelay);
	// 	}
	// }
}
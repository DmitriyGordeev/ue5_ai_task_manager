// Fill out your copyright notice in the Description page of Project Settings.

#include "AIBaseTask.h"
#include "AITaskManager.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Kismet/KismetSystemLibrary.h"


// TODO: сделать общие коментарии после тестов


void UAITaskManager::Start()
{
	bStarted = true;
	Recalculate();
}

void UAITaskManager::Recalculate()
{
	UE_LOG(LogTemp, Log, TEXT("TaskManager::Recalculate()"));
	if (Tasks.IsEmpty())
		return;

	if (ActiveTask)
		ActiveTask->Reset();

	UAIBaseTask* Winner = nullptr;
	float MaxProbaSoFar = -1.0f;
	// int WinnerIndex = -1;
	
	// for(auto it = Tasks.begin();
	// 	it != Tasks.end();
	// 	++it)
	// {
	// 	float Proba = (*it)->FindProba(AIOwner.Get(), ContextData);
	// 	if (MaxProba < Proba)
	// 	{
	// 		MaxProba = Proba;
	// 		Winner = (*it);
	// 		
	// 		UE_LOG(LogTemp, Log,
	// 			TEXT("Proba = %f, Winner Name = %s"),
	// 			Proba,
	// 			*Winner->GetName());
	// 	}
	// }

	float Proba = 0.0f;
	for(auto i = 0; i < Tasks.Num(); i++)
	{
		Proba = Tasks[i]->FindProba(AIOwner.Get(), ContextData);
		if (Proba >= MaxProbaSoFar)
		{
			MaxProbaSoFar = Proba;
			Winner = Tasks[i];
			// WinnerIndex = i;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Winner Task Name = %s"), *Winner->GetName());
	if (!Winner)
		return;
	
	ActiveTask = Winner;
	Winner->Start();
}

bool UAITaskManager::TryInterruptActiveTask()
{
	UE_LOG(LogTemp, Log, TEXT("RequestInterruptActive"));
	bWaitingForActiveTaskInterrupted = true;
	if (!ActiveTask)
		return true;

	if (AIOwner.IsValid())
		ActiveTask->OnInterruptedResponse(AIOwner.Get());

	return ActiveTask->IsInterrupted();
}

void UAITaskManager::AddTask(UAIBaseTask* Task)
{
	if (!Task)
		return;
	Task->SetTaskManager(this);
	Tasks.Add(Task);
	UE_LOG(LogTemp, Log, TEXT("Task was added"));
}


void UAITaskManager::Tick(float DeltaTime)
{
	// TODO: expose tick frequency float (time sec)
	
	UE_LOG(LogTemp, Log, TEXT("TaskManager tick | active task name = %s"), *ActiveTask->GetName());
	if (!bStarted)
		return;

	// if (ActiveTask && bWaitingForActiveTaskInterrupted)
	// {
	// 	if (ActiveTask->IsCompleted() || ActiveTask->IsInterrupted())
	// 	{
	// 		UE_LOG(LogTemp, Log, TEXT("Task Interrupted"));
	// 		bWaitingForActiveTaskInterrupted = false;
	// 		
	// 		// TODO: action (Recalculate?)
	// 		
	// 	}
	// }

	if (!ActiveTask)
		return;

	if (ActiveTask->IsCompleted())
	{
		// TODO: внести ContextData внутрь 
		//  Recalculate();
		return;
	}

	if (bWaitingForActiveTaskInterrupted)
	{
		if (ActiveTask->IsCompleted() || ActiveTask->IsInterrupted())
		{
			UE_LOG(LogTemp, Log, TEXT("Task Interrupted"));
			bWaitingForActiveTaskInterrupted = false;
			Recalculate();
		}
	}
	
}

bool UAITaskManager::IsTickable() const
{
	return !IsTemplate(RF_ClassDefaultObject) && bStarted;
}

bool UAITaskManager::IsTickableInEditor() const
{
	return false;
}

bool UAITaskManager::IsTickableWhenPaused() const
{
	return false;
}

TStatId UAITaskManager::GetStatId() const
{
	return TStatId();
}

UWorld* UAITaskManager::GetWorld() const
{
	// Return null if called from the CDO, or if the outer is being destroyed
	if (!HasAnyFlags(RF_ClassDefaultObject) &&
		!GetOuter()->HasAnyFlags(RF_BeginDestroyed) &&
		!GetOuter()->IsUnreachable())
	{
		// Try to get the world from the owning actor if we have one
		AActor* Outer = GetTypedOuter<AActor>();
		if (Outer != nullptr)
		{
			return Outer->GetWorld();
		}
	}
	// Else return null - the latent action will fail to initialize
	return nullptr;
}
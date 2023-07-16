// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBaseTask.h"

void UAIBaseTask::Start()
{
	UE_LOG(LogTemp, Log, TEXT("UAIBaseTask::Start()"));
	bStarted = true;
	OnExecute();
}

void UAIBaseTask::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("CustomTickable Tick"));
	OnTick();
}

bool UAIBaseTask::IsTickable() const
{
	return !IsTemplate(RF_ClassDefaultObject) && bStarted;
}

bool UAIBaseTask::IsTickableInEditor() const
{
	return false;
}

bool UAIBaseTask::IsTickableWhenPaused() const
{
	return false;
}

void UAIBaseTask::MarkCompleted()
{
	bCompleted = true;
	UE_LOG(LogTemp, Log, TEXT("Task marked as completed"));
	
	// TODO: notify up to TaskManager
}


void UAIBaseTask::MarkInterrupted()
{
	bInterrupted = true;
	UE_LOG(LogTemp, Log, TEXT("Task marked as interrupted"));
	
	// TODO: notify up to TaskManager
}


TStatId UAIBaseTask::GetStatId() const
{
	return TStatId();
}

UWorld* UAIBaseTask::GetWorld() const
{
	// return GetOuter()->GetWorld();
	
	// Return null if called from the CDO, or if the outer is being destroyed
	if (!HasAnyFlags(RF_ClassDefaultObject) &&
		!GetOuter()->HasAnyFlags(RF_BeginDestroyed) &&
		!GetOuter()->IsUnreachable())
	{
		// Try to get the world from the owning actor if we have one
		AActor* Outer = GetTypedOuter<AActor>();
		if (Outer != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("AIBaseTask::GetWorld() => Sucessfully GetWorld()"));
			return Outer->GetWorld();
		}
	}
	
	// Else return null - the latent action will fail to initialize
	UE_LOG(LogTemp, Display, TEXT("World is null"));
	return nullptr;
}



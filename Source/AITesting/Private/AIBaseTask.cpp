// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBaseTask.h"

void UAIBaseTask::Start()
{
	UE_LOG(LogTemp, Log, TEXT("UAIBaseTask::Start()"));
	bStarted = true;
	OnStart.Broadcast();
}

void UAIBaseTask::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("CustomTickable Tick"));
	OnTick.Broadcast();
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

TStatId UAIBaseTask::GetStatId() const
{
	return TStatId();
}

UWorld* UAIBaseTask::GetWorld() const
{
	return GetOuter()->GetWorld();
}

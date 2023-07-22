// Fill out your copyright notice in the Description page of Project Settings.


#include "AITaskManager.h"


void UAITaskManager::Start()
{
	UE_LOG(LogTemp, Log, TEXT("TaskManager::Start()"));
	Recalculate();
}

void UAITaskManager::Recalculate()
{
	
	// if (UObject* OuterObject = GetOuter())
	// {
	// 	AAIController* AIC = Cast<AAIController>(OuterObject);
	// 	UE_LOG(LogTemp, Log, TEXT("OuterName = %s"), *AIC->GetName());
	// }

	
	UE_LOG(LogTemp, Log, TEXT("TaskManager::Recalculate()"));
	if (Tasks.IsEmpty())
		return;

	float MaxProba = 0.0f;
	UAIBaseTask* Winner = nullptr;
	for(auto it = Tasks.begin();
		it != Tasks.end();
		++it)
	{
		float Proba = (*it)->FindProba();
		if (MaxProba < Proba)
		{
			MaxProba = Proba;
			Winner = (*it);
		}
	}

	if (!Winner)
		return;
	
	ActiveTask = Winner;
	Winner->Start();
}

bool UAITaskManager::TryInterruptActive()
{
	UE_LOG(LogTemp, Log, TEXT("RequestInterruptActive"));
	if (!ActiveTask)
		return true;
	
	ActiveTask->OnInterruptedResponse();
	
	// TODO: как работать с долгим InterruptedResponse() ?
	//	юзер может вызывать асинхронные функции

	if (ActiveTask->IsInterrupted())
		return true;

	return false;
}

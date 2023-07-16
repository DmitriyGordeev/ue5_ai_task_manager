// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTickableObject.h"

// void UCustomTickableObject::CustomTickable(const UObject* WorldContext, FString& Message)
// {
// 	UCustomTickableObject* Instance = NewObject<UCustomTickableObject>();
// 	Message = "Some message";
// }


void UCustomTickableObject::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("CustomTickable Tick"));
}

bool UCustomTickableObject::IsTickable() const
{
	return !IsTemplate(RF_ClassDefaultObject);
}

bool UCustomTickableObject::IsTickableInEditor() const
{
	return false;
}

bool UCustomTickableObject::IsTickableWhenPaused() const
{
	return false;
}

TStatId UCustomTickableObject::GetStatId() const
{
	return TStatId();
}

UWorld* UCustomTickableObject::GetWorld() const
{
	return GetOuter()->GetWorld();
}


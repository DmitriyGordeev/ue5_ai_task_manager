// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTickableObject.h"

void UCustomTickableObject::CallTimer(float Delay)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		this,
		&UCustomTickableObject::TestTimer,
		Delay,
		false);
}

void UCustomTickableObject::TestTimer()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "HelloWorld");
}

void UCustomTickableObject::Tick(float DeltaTime)
{
	TestCounter += DeltaTime;
	GEngine->AddOnScreenDebugMessage(1, 2,
		FColor::Green, FString::SanitizeFloat(TestCounter));

}

bool UCustomTickableObject::IsTickable() const
{
	// return FTickableGameObject::IsTickable();
	return true;
}

bool UCustomTickableObject::IsTickableInEditor() const
{
	// return FTickableGameObject::IsTickableInEditor();
	return false;
}

bool UCustomTickableObject::IsTickableWhenPaused() const
{
	// return FTickableGameObject::IsTickableWhenPaused();
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

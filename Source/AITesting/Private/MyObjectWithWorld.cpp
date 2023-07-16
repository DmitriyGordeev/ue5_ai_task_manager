// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectWithWorld.h"


UWorld* UMyObjectWithWorld::GetWorld() const
{
	UE_LOG(LogTemp, Log, TEXT("Start GetWorld()"));
	
	// Return null if called from the CDO, or if the outer is being destroyed
	if (!HasAnyFlags(RF_ClassDefaultObject) &&
		!GetOuter()->HasAnyFlags(RF_BeginDestroyed) &&
		!GetOuter()->IsUnreachable())
	{
		// Try to get the world from the owning actor if we have one
		AActor* Outer = GetTypedOuter<AActor>();
		if (Outer != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Sucessfully GetWorld()"));
			return Outer->GetWorld();
		}
	}
	
	// Else return null - the latent action will fail to initialize
	UE_LOG(LogTemp, Display, TEXT("World is null"));
	return nullptr;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomTickableObject.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionEvent);

/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UCustomTickableObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// UFUNCTION(DisplayName="CustomTickable", BlueprintCallable, meta=(WorldContext="WorldContext"))
	// static void CustomTickable(const UObject* WorldContext, FString& Message);
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;

	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;
};

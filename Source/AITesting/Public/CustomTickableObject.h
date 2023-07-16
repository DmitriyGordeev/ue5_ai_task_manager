// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomTickableObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UCustomTickableObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CallTimer(float Delay);

	void TestTimer();

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;

	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;

	float TestCounter;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIBaseTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDelegate);

/**
 * 
 */
UCLASS()
class AITESTING_API UAIBaseTask : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	virtual void Start();
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FEventDelegate OnStart;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FEventDelegate OnTick;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FEventDelegate OnInterruptedResponse;

	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;

protected:
	bool bStarted;
};

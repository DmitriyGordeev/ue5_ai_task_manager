// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIBaseTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDelegate);

/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UAIBaseTask : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnExecute();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTick();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInterruptedResponse();
	
	UFUNCTION(BlueprintCallable)
	virtual void Start();

	// TODO: keep AIC as Outer (Owner) to keep for WorldContext ?
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;

	UFUNCTION(BlueprintCallable)
	virtual void MarkCompleted();

	UFUNCTION(BlueprintCallable)
	virtual void MarkInterrupted();

	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;

protected:
	bool bStarted {false};
	bool bCompleted {false};
	bool bInterrupted {false};
};

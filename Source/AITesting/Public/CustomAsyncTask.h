// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "UObject/NoExportTypes.h"
#include "CustomAsyncTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionEvent);

/**
 * 
 */
UCLASS()
class AITESTING_API UCustomAsyncTask : public UCancellableAsyncAction, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(DisplayName="CustomAsyncTaskNode", BlueprintCallable, meta=(WorldContext="WorldContext"))
	static UCustomAsyncTask* CreateCustomAsyncTask(const UObject* WorldContext);
	

	UPROPERTY(BlueprintAssignable)
	FActionEvent OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FActionEvent OnComplete;

	UPROPERTY(BlueprintAssignable)
	FActionEvent OnInterrupted;

	// TODO: OnFail ?
	
	virtual void Activate() override;
	virtual void Cancel() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;

	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;
};

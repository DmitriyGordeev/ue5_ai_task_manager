// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "MyCancellableAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyDelayAsyncActionEvent);

/**
 * 
 */
UCLASS()
class AITESTING_API UMyCancellableAsyncAction : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:
	UFUNCTION(DisplayName="MyCancellableAsyncNode", BlueprintCallable, meta=(WorldContext="WorldContext"))
	static UMyCancellableAsyncAction* MyCancellableAsyncAction(const UObject* WorldContext, float DelayTime);

	UPROPERTY(BlueprintAssignable)
	FMyDelayAsyncActionEvent OnComplete;

	UPROPERTY(BlueprintAssignable)
	FMyDelayAsyncActionEvent OnFail;

	virtual void Activate() override;
	virtual void Cancel() override;

	virtual UWorld* GetWorld() const override
	{
		return ContextWorld.IsValid() ? ContextWorld.Get() : nullptr;
	}

private:
	TWeakObjectPtr<UWorld> ContextWorld {nullptr};
	float DelayTime {0.0f};
	FTimerHandle OngoingDelay;
};

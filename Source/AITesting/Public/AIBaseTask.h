// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "AITaskManager.h"
#include "UObject/NoExportTypes.h"
#include "AIBaseTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDelegate);

class AAIController;
class UAITaskManager;

/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UAIBaseTask : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnExecute(AAIController* Controller);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTick(AAIController* Controller);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInterruptedResponse(AAIController* Controller);
	
	UFUNCTION(BlueprintCallable)
	virtual void Start();
	
	UFUNCTION(BlueprintNativeEvent)
	float FindProba(AAIController* Controller, UObject* ContextData);
	
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

	virtual bool IsCompleted() const;
	virtual bool IsInterrupted() const;

	virtual float GetProba() const;

	UFUNCTION()
	void SetTaskManager(UAITaskManager* TaskManager);
	
	UFUNCTION(BlueprintCallable)
	AAIController* GetAIController();

	virtual void Reset();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAITaskManager> TaskManager;

	UPROPERTY(BlueprintReadOnly)
	bool bRunning {false};

	UPROPERTY(BlueprintReadOnly)
	bool bCompleted {false};

	UPROPERTY(BlueprintReadOnly)
	bool bInterrupted {false};
	float Proba {0.0f};
};

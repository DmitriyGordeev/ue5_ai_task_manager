// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBaseTask.h"
#include "AIController.h"
#include "AITaskManager.generated.h"


class AAIController;

/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UAITaskManager : public UObject
{
	GENERATED_BODY()

public:
	
	// нужно было включить AIModule в *.Build.cs
	// иначе доступ к AAIController вызовет 'unresolved external symbol error'
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AAIController> AIOwner;
	
	/* TaskManager's entry point (equivalent of RunBehaviorTree) */
	UFUNCTION(BlueprintCallable)
	virtual void Start();

	/* Find next relevant task to execute */
	UFUNCTION(BlueprintCallable)
	virtual void Recalculate();
	
	/* Asks current task for interruption - it's up to Task to respond:
	 * if task hasn't responded - it's continue running until marked Completed */
	UFUNCTION(BlueprintCallable)
	virtual bool TryInterruptActive();
	
	// virtual void Tick();
	
	// OnBeforeTaskChange
	// OnAfterTaskChange

	UFUNCTION(BlueprintCallable)
	void AddTask(UAIBaseTask* Task);
	
	// TODO: move to protected
	UPROPERTY(BlueprintReadWrite)
	UAIBaseTask* ActiveTask;


protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<UAIBaseTask*> Tasks;
	
};

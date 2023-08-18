// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBaseTask.h"
#include "AIController.h"
#include "AIReaction.h"
#include "Containers/Deque.h"
#include "Templates/Tuple.h"
#include "AITaskManager.generated.h"


class AAIController;

/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UAITaskManager : public UObject, public FTickableGameObject
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
	virtual bool TryInterruptActiveTask();
	
	// virtual void Tick();
	
	// OnBeforeTaskChange
	// OnAfterTaskChange

	/** Adds task to TaskManager
	 * returns index of just added task in the array of tasks,
	 * if task null object returns -1
	 * */
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "TaskIndex") int AddTask(UAIBaseTask* Task);
	
	// TODO: move to protected
	UPROPERTY(BlueprintReadWrite)
	UAIBaseTask* ActiveTask;

	UFUNCTION(BlueprintCallable)
	void ConsumeReaction(int32 ReactionType);

	// -------- FTickableGameObject functions --------------
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;


	// TODO: Move to protected ?
	UPROPERTY(BlueprintReadWrite)
	UObject* ContextData;

	/** Tells TaskManager that Task with TaskIndex1 should have greater priority
	 * over TaskIndex2 in case both tasks have the same probas */
	UFUNCTION(BlueprintCallable)
	virtual void AddPairWisePriority(int HigherPriorityTaskIndex, int LowerPriorityTaskIndex);
	
	bool CheckRecalculateCooldownIsReady();

	bool FindReactionInQueue(UAIBaseTask* FromTask, int32 EnumIndex) const;


protected:
	TTuple<UAIBaseTask*, int> CompareTwoTasks(UAIBaseTask* T1, UAIBaseTask* T2, int Index1, int Index2);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<UAIBaseTask*> Tasks;
	
	bool bStarted {false};

	UPROPERTY(BlueprintReadOnly)
	bool bWaitingForActiveTaskInterrupted {false};

	// pair-wise priority for each task
	TMap<TTuple<int, int>, int> PriorityMatrix;
	
	// TDeque<TTuple<int32, bool>> ReactionQueue;
	TMap<int32, bool> Reactions;
	
	int64 LastRecalcUnixTime {0};
};

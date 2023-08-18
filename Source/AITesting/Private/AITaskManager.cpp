// Fill out your copyright notice in the Description page of Project Settings.

#include "AITaskManager.h"
#include "AIBaseTask.h"
#include "MyRunnable.h"


#include "SAdvancedTransformInputBox.h"
#include "Containers/Deque.h"
#include "Engine/RendererSettings.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Kismet/KismetSystemLibrary.h"


// TODO: сделать общие коментарии после тестов


void UAITaskManager::Start()
{
	bStarted = true;
	Recalculate();
}

// TODO: нужно отрефакторить
void UAITaskManager::Recalculate()
{
	UE_LOG(LogTemp, Log, TEXT("TaskManager::Recalculate()"));
	if (Tasks.IsEmpty())
		return;
	
	if (!CheckRecalculateCooldownIsReady())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[Before update] LastRecalcUnitTime = %lld"), LastRecalcUnixTime);
	LastRecalcUnixTime = GetCurrentMilliseconds();
	UE_LOG(LogTemp, Log, TEXT("[After update] LastRecalcUnitTime = %lld"), LastRecalcUnixTime);
	
	UAIBaseTask* Winner = nullptr;
	int WinnerIndex = -1;


	// TODO: может ли быть так, что задача имеет вероятность Proba = 0.0,
	//	но флаг GetConsumedReaction = true, а все остальные задачи имеют этот флаг false,
	//	но proba не ноль
	
	
	for(auto i = 0; i < Tasks.Num(); i++)
	{
		// early stop for comparing the task with others if specified explicitly (in BP)
		if (Tasks[i]->ShouldBeIgnored(AIOwner.Get(), ContextData))
		{
			UE_LOG(LogTemp, Log, TEXT("Task %s ShouldBeIgnored = true"), *Tasks[i]->GetName());
			continue;
		}
		
		Tasks[i]->ExtractProba(AIOwner.Get(), ContextData);
		UE_LOG(LogTemp, Log, TEXT("Task %s -> ExtractProba = %f"), *Tasks[i]->GetName(), Tasks[i]->GetProba());
		
		if (!Winner)
		{
			Winner = Tasks[i];
			WinnerIndex = i;
			continue;
		}

		UE_LOG(LogTemp, Log,
			TEXT("Task %s : GetConsumedReaction() = %i"),
			*Tasks[i]->GetName(),
			Tasks[i]->GetConsumedReaction()
			);

		if (Tasks[i]->GetConsumedReaction())
		{
			// if both winner and current task have consumed some reaction, we need
			// to treat them as equal
			if (Winner->GetConsumedReaction())
			{
				TTuple<UAIBaseTask*, int> Tuple = CompareTwoTasks(Tasks[i], Winner, i, WinnerIndex);
				Winner = Tuple.Key;
				WinnerIndex = Tuple.Value;
				continue;
			}

			// if Winner hasn't consumed any reaction,
			// current task Tasks[i] should have more priority
			// (except only if Tasks[i] itself has Proba = 0)
			if (Tasks[i]->GetProba() > 0.0f)
			{
				Winner = Tasks[i];
				WinnerIndex = i;
				continue;
			}
		}

		// If current task Tasks[i] hasn't consumed any reaction,
		// but winner has, Winner has more priority
		if (Winner->GetConsumedReaction())
			continue;
		
		// If both haven't consumed any reaction - compare them as equal
		TTuple<UAIBaseTask*, int> Tuple = CompareTwoTasks(Tasks[i], Winner, i, WinnerIndex);
		Winner = Tuple.Key;
		WinnerIndex = Tuple.Value;
	}

	
	// Cleanup all reaction from Reactions map which were marked Consumed
	const int64 TimeNow = GetCurrentMilliseconds();
	for(const auto& t : Reactions)
	{
		// remove reaction if it was Consumed by whatever task,
		// or if it's expired
		if (t.Value.Consumed || TimeNow >= t.Value.LifeTimeMs + t.Value.StartTime)
		{
			Reactions.Remove(t.Key);
			UE_LOG(LogTemp, Log, TEXT("EReaction %i was removed"), t.Key);
		}
	}
	
	
	if (!Winner)
	{
		UE_LOG(LogTemp, Log, TEXT("Winner is null"));
		return;
	}
	
	if (Winner->GetProba() <= 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Winner(%s)->GetProba = 0.0f"), *Winner->GetName());
		return;
	}
	
	if (ActiveTask && Winner == ActiveTask)
	{
		if (ActiveTask->bShouldRestartIfWinnerAgain)
		{
			UE_LOG(LogTemp, Log, TEXT("Restaring the same Task because it's a winner again"));
			ActiveTask->Reset();
			ActiveTask->Start();
		}
		return;
	}
	
	if (ActiveTask)
		ActiveTask->Reset();
	
	UE_LOG(LogTemp, Log, TEXT("Winner Task Name = %s"), *Winner->GetName());
	
	ActiveTask = Winner;
	Winner->Start();
}

bool UAITaskManager::TryInterruptActiveTask()
{
	UE_LOG(LogTemp, Log, TEXT("RequestInterruptActive"));
	bWaitingForActiveTaskInterrupted = true;
	if (!ActiveTask)
		return true;

	if (AIOwner.IsValid())
		ActiveTask->OnInterruptedResponse(AIOwner.Get());

	return ActiveTask->IsInterrupted();
}

int UAITaskManager::AddTask(UAIBaseTask* Task)
{
	if (!Task)	// TODO: добавить Task->IsValidLowLevel() ?
		return -1;
	Task->SetTaskManager(this);
	Tasks.Add(Task);
	UE_LOG(LogTemp, Log, TEXT("Task was added"));
	return Tasks.Num() - 1;
}


void UAITaskManager::ConsumeReaction(int32 ReactionType, int64 LifeTimeMs)
{	
	
	LifeTimeMs = LifeTimeMs < 0 ? 0 : LifeTimeMs;
	
	Reactions.Add(ReactionType, {
		ReactionType,
		false,
		GetCurrentMilliseconds(),
		LifeTimeMs});

	UE_LOG(LogTemp, Log, TEXT("Consuming reaction type = %i, StartTime = %lld"), ReactionType, GetCurrentMilliseconds());

	// todo: подумать про блок по времени
	Recalculate();
}

void UAITaskManager::Tick(float DeltaTime)
{
	// TODO: expose tick frequency float (time sec)
	
	if (!bStarted)
		return;

	if (ActiveTask)
		UE_LOG(LogTemp, Log, TEXT("TaskManager tick | active task name = %s"), *ActiveTask->GetName());

	if (!ActiveTask)
		return;

	if (ActiveTask->IsCompleted())
	{
		Recalculate();
		return;
	}

	if (bWaitingForActiveTaskInterrupted)
	{
		if (ActiveTask->IsCompleted() || ActiveTask->IsInterrupted())
		{
			UE_LOG(LogTemp, Log, TEXT("Task Interrupted"));
			bWaitingForActiveTaskInterrupted = false;
			Recalculate();
		}
	}
	
}

bool UAITaskManager::IsTickable() const
{
	return !IsTemplate(RF_ClassDefaultObject) && bStarted;
}

bool UAITaskManager::IsTickableInEditor() const
{
	return false;
}

bool UAITaskManager::IsTickableWhenPaused() const
{
	return false;
}

TStatId UAITaskManager::GetStatId() const
{
	return TStatId();
}

UWorld* UAITaskManager::GetWorld() const
{
	// Return null if called from the CDO, or if the outer is being destroyed
	if (!HasAnyFlags(RF_ClassDefaultObject) &&
		!GetOuter()->HasAnyFlags(RF_BeginDestroyed) &&
		!GetOuter()->IsUnreachable())
	{
		// Try to get the world from the owning actor if we have one
		AActor* Outer = GetTypedOuter<AActor>();
		if (Outer != nullptr)
		{
			return Outer->GetWorld();
		}
	}
	// Else return null - the latent action will fail to initialize
	return nullptr;
}

void UAITaskManager::AddPairWisePriority(int HigherPriorityTaskIndex, int LowerPriorityTaskIndex)
{
	if (HigherPriorityTaskIndex >= Tasks.Num() || LowerPriorityTaskIndex >= Tasks.Num())
		return;

	if (HigherPriorityTaskIndex < 0 || LowerPriorityTaskIndex < 0)
		return;

	if (HigherPriorityTaskIndex == LowerPriorityTaskIndex)
		return;

	PriorityMatrix.Add(TTuple<int, int>(HigherPriorityTaskIndex, LowerPriorityTaskIndex), 1);
	PriorityMatrix.Add(TTuple<int, int>(LowerPriorityTaskIndex, HigherPriorityTaskIndex), -1);
}

bool UAITaskManager::CheckRecalculateCooldownIsReady()
{
	if (LastRecalcUnixTime == 0)
	{
		LastRecalcUnixTime = GetCurrentMilliseconds();
		return true;
	}
	return (GetCurrentMilliseconds() - LastRecalcUnixTime) > 0;
}

bool UAITaskManager::TryActivateReaction(UAIBaseTask* FromTask, int32 ReactionType)
{
	if (Reactions.Contains(ReactionType))
	{
		if (FromTask)
			FromTask->SetConsumedReaction(true);
		Reactions[ReactionType].Consumed = true;
		return true;
	}
	return false;
}

TTuple<UAIBaseTask*, int> UAITaskManager::CompareTwoTasks(UAIBaseTask* T1, UAIBaseTask* T2, int Index1, int Index2)
{
	if (!T1 || !T2)
		return {nullptr, -1};

	float Proba1 = T1->GetProba();
	float Proba2 = T2->GetProba();

	if (Proba1 > Proba2)
	{
		T2->SetConsumedReaction(false);
		return {T1, Index1};
	}
	
	if (Proba1 == Proba2)
	{
		auto Pair = PriorityMatrix.Find(TTuple<int, int>(Index1, Index2));
		if (Pair && *Pair < 0)
		{
			if (*Pair < 0)
			{
				T1->SetConsumedReaction(false);
				return {T2, Index2};
			}
				
			if (*Pair > 0)
			{
				T2->SetConsumedReaction(false);
				return {T1, Index1};
			}
				
		}

		auto p = FMath::FRand();
		UE_LOG(LogTemp, Log, TEXT("Selecting random task of two, p = %f"), p);
		if (p > 0.5f)
		{
			T2->SetConsumedReaction(false);
			return {T1, Index1};
		}
		else
		{
			T1->SetConsumedReaction(false);
			return {T2, Index2};
		}
	}

	T1->SetConsumedReaction(false);
	return {T2, Index2};
}

int64 UAITaskManager::GetCurrentMilliseconds()
{
	const auto CurrentTime = FDateTime::Now();
	return CurrentTime.GetMillisecond() +
			CurrentTime.GetSecond() * 1000 +
			CurrentTime.GetMinute() * 60 * 1000;
}
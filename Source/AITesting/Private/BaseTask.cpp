// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTask.h"

void UBaseTask::Foo()
{
	// FTimerHandle TimerHandle;
	//
	// // // with class method:
	// // FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::Bar, 1.12f);
	// // GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, true, -1.0f);
	//
	// // with Lambda expression:
	// auto Callback = [](float value)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("Bar function %f"), value);
	// };
	// FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, Callback, 1.0f);
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, true);
}

void UBaseTask::Bar(float value)
{
	// UE_LOG(LogTemp, Log, TEXT("Bar function %f"), value);
}



void UBaseTask::FooWithAsync()
{
	int a = 0;
	UE_LOG(LogTemp, Log, TEXT("FooWithAsync starts"));
	
	// AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&a] ()
	// {
	// 	for(int i = 0; i < 1000; i++)
	// 	{
	// 		for(int j = 0; j < 1000; j++)
	// 			a = i;
	// 		a = 0;
	// 	}
	//
	// 	// Scheduling back to GameThread to do something with results
	// 	AsyncTask(ENamedThreads::GameThread, [&a]()
	// 	{
	// 		UE_LOG(LogTemp, Log, TEXT("Finished"));
	// 	});
	// });

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, []
	{
		UE_LOG(LogTemp, Log, TEXT("AsyncTask starts"));

		int value = 0;
		for(int i = 0; i < 1000000; i++)
		{
			for(int j = 0; j < 100000; j++)
			{
				value = j;
			}
			value += i;
		}
		
		AsyncTask(ENamedThreads::GameThread, [value]
		{
			UE_LOG(LogTemp, Log, TEXT("AsyncTask Response to the GameThread, value = %i"), value);
		});
	});
	
	UE_LOG(LogTemp, Log, TEXT("FooWithAsync finished, but AsyncTask is probably running"));
}


void UBaseTask::Execute_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Execute implementation"));
}

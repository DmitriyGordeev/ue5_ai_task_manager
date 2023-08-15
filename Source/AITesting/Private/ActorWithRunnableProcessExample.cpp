// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorWithRunnableProcessExample.h"

// Sets default values
AActorWithRunnableProcessExample::AActorWithRunnableProcessExample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// TODO: если вызвать MyRunnable здесь, то будет несколько вызовов,
	//		возможно связано с CDO ?

}

// Called when the game starts or when spawned
void AActorWithRunnableProcessExample::BeginPlay()
{
	Super::BeginPlay();

	// Start some runnable process on EventBeginPlay
	RunnableObject = MakeShared<MyRunnable>();
	FRunnableThread* Thread = FRunnableThread::Create(
		RunnableObject.Get(),
		TEXT("MyCustomThread"));
}

// Called every frame
void AActorWithRunnableProcessExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


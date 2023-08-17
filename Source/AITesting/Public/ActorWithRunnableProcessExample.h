// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyRunnable.h"
#include "GameFramework/Actor.h"
#include "ActorWithRunnableProcessExample.generated.h"

UCLASS()
class AITESTING_API AActorWithRunnableProcessExample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorWithRunnableProcessExample();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Runnable object holding some function inside to run
	TSharedPtr<MyRunnable> RunnableObject;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

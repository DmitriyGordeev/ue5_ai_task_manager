// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
// #include "Async/Async.h"
#include "BaseTask.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AITESTING_API UBaseTask : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Foo();
	
	void Bar(float value);
	
	UFUNCTION(BlueprintCallable)
	void FooWithAsync();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Execute();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObjectWithWorld.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AITESTING_API UMyObjectWithWorld : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Execute();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIReaction.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class AITESTING_API UAIReaction : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	UEnum* ReactionType {nullptr};

	UPROPERTY(BlueprintReadWrite)
	bool Consumed {false};
};



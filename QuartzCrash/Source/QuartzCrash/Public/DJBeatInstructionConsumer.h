// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DJBeatInstructionConsumer.generated.h"

class UQuartzClockHandle;
class UDJBeatInstruction;

/**
 * 
 *		UDJBeatInstructionConsumer
 * 
 *		Consumes and Registers Beat System Track Requests with Beat Manager
 */
UCLASS(Blueprintable, ClassGroup = ("Beat System"), meta=(BlueprintSpawnableComponent))
class QUARTZCRASH_API UDJBeatInstructionConsumer : public USceneComponent
{
	GENERATED_BODY()

public:
	UDJBeatInstructionConsumer();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UQuartzClockHandle> BeatClockHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, NoClear)
	TArray<TObjectPtr<UDJBeatInstruction>> BeatInstructions;

	UFUNCTION(BlueprintCallable)
	void ActivateInstructions();


private:
	UPROPERTY()
	FOnQuartzCommandEventBP _OnLoopStartDelegate;
	UFUNCTION()
	void _OnLoopStartCallback(EQuartzCommandDelegateSubType EventType, FName Name);

	void _SubscribeToLoopStart();
	void _QueueInstructions();
};

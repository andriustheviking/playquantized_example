// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Sound/QuartzQuantizationUtilities.h"

#include "DJBeatInstruction.generated.h"

class UQuartzClockHandle;


UENUM(BlueprintType)
enum class EDJBeatInstructionIntervalMode : uint8
{
	OnBar		UMETA(DisplayName = "Every Bar"),
	OnBeat		UMETA(DisplayName = "Every Beat"),
	OnInterval	UMETA(DisplayName = "Every Interval"),
};

/***************************************************************
 * 
 *					Abstract Base Class
 *		
 **************************************************************/

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, CollapseCategories, DisplayName = "Play Audio")
class QUARTZCRASH_API UDJBeatInstruction : public UObject
{
	GENERATED_BODY()
public:
	UDJBeatInstruction();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> Audio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuartzCommandQuantization Interval = EQuartzCommandQuantization::Beat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Multiplier = 1.f;

	virtual void Activate(UQuartzClockHandle* ClockHandle);


	void _QueueAudioBeforeLoopStart();

protected:
	UPROPERTY()
	UQuartzClockHandle* _ClockHandle;

private:
	UPROPERTY()
	TObjectPtr<USoundConcurrency> _ConcurrencySettings;
	UPROPERTY()
	TObjectPtr<UAudioComponent> _AudioComponent;

	// Quartz
	UPROPERTY()
	FOnQuartzCommandEventBP _OnAudioEventDelegate;
	UFUNCTION()
	void _OnAudioEventDelegateCallback(EQuartzCommandDelegateSubType EventType, FName Name);
	
	// Helpers
	UAudioComponent* _GetAudioComponent();
};
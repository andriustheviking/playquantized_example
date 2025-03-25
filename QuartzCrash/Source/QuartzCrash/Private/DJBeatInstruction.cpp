// Fill out your copyright notice in the Description page of Project Settings.


#include "DJBeatInstruction.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MetasoundSource.h"
#include "MetasoundParameterPack.h"

#include "Quartz/AudioMixerClockHandle.h"
#include "Sound/QuartzQuantizationUtilities.h"
#include "Sound/SoundConcurrency.h"

#include "DJLog.h"

/**************************************************************
 *
 *					Abstract Base
 *
 **************************************************************/

UDJBeatInstruction::UDJBeatInstruction()
{
	Intervals.Add(EQuartzCommandQuantization::None);

	_ConcurrencySettings = CreateDefaultSubobject<USoundConcurrency>(TEXT("SoundConcurrency"));

	_OnAudioEventDelegate.BindDynamic(this, &UDJBeatInstruction::_OnAudioEventDelegateCallback);
}


void UDJBeatInstruction::Activate(UQuartzClockHandle* ClockHandle)
{
	DJ_RETURN_IF(!IsValid(ClockHandle));

	_ClockHandle = ClockHandle;
}


// Quartz Callbacks

void UDJBeatInstruction::_QueueAudioBeforeLoopStart()
{
	DJ_RETURN_IF(!IsValid(_ClockHandle));

	for (auto interval : Intervals)
	{
		if (interval == EQuartzCommandQuantization::None)
		{
			DJLog(Error, "Encountered invalid Interval. Skipping.")
			continue;
		}

		FQuartzQuantizationBoundary quantBoundary;

		quantBoundary.bFireOnClockStart = (Multiplier == 1);
		quantBoundary.Multiplier = Multiplier;
		quantBoundary.Quantization = interval;
		quantBoundary.CountingReferencePoint = EQuarztQuantizationReference::TransportRelative;

		DJLog(Warning, "Queueing Audio - multiplier=%f reference=%s interval=%s", quantBoundary.Multiplier, *UEnum::GetValueAsString(quantBoundary.CountingReferencePoint), *UEnum::GetValueAsString(quantBoundary.Quantization));

		auto audioComponent = _GetAudioComponent();
		DJ_RETURN_IF(!IsValid(audioComponent));

		audioComponent->PlayQuantized(this, _ClockHandle, quantBoundary, _OnAudioEventDelegate);
	}
}


void UDJBeatInstruction::_OnAudioEventDelegateCallback(EQuartzCommandDelegateSubType EventType, FName Name)
{
	if (EventType == EQuartzCommandDelegateSubType::CommandOnStarted)
	{
		DJLog(Error, "_OnAudioEventDelegateCallback EventType=%s", *UEnum::GetValueAsString(EventType));
	}
}


// Helpers


UAudioComponent* UDJBeatInstruction::_GetAudioComponent()
{ 
	DJ_RETURN_NULLPTR_IF(Audio == nullptr);
	
	if (_AudioComponent)
	{
		return _AudioComponent;
	}

	const int maxConcurrent = 10 * Intervals.Num();
	_ConcurrencySettings->Concurrency.MaxCount = maxConcurrent;

	DJLog(Warning, "Creating new audio component with %d concurrent count.", maxConcurrent);

	_AudioComponent = UGameplayStatics::CreateSound2D(
		this,
		Audio,
		1.0,
		1.0,
		0.0,
		nullptr,
		false,
		false
	);	

	_AudioComponent->bCanPlayMultipleInstances = true;

	return _AudioComponent;
}
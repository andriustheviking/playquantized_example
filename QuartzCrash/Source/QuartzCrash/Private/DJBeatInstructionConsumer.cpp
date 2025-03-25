// Fill out your copyright notice in the Description page of Project Settings.


#include "DJBeatInstructionConsumer.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Quartz/AudioMixerClockHandle.h"

#include "DJLog.h"
#include "DJBeatInstruction.h"

/*
*	UDJBeatInstructionConsumer
*/


UDJBeatInstructionConsumer::UDJBeatInstructionConsumer()
{
	_OnLoopStartDelegate.BindDynamic(this, &UDJBeatInstructionConsumer::_OnLoopStartCallback);
}

void UDJBeatInstructionConsumer::ActivateInstructions()
{
	ensureMsgf(BeatClockHandle != nullptr, TEXT("UDJBeatInstructionConsumer::Add - Attempted to Add beat without BeatClockHandle"));
	DJ_RETURN_IF(BeatClockHandle == nullptr);

	for (auto instruction : BeatInstructions)
	{
		if (!IsValid(instruction))
		{
			DJLog(Error, "Encountered invalidated instruction pointer");
			continue;
		}

		instruction->Activate(BeatClockHandle);
	}

	_SubscribeToLoopStart();
}

void UDJBeatInstructionConsumer::_OnLoopStartCallback(EQuartzCommandDelegateSubType EventType, FName Name)
{
	DJLog(Warning, "Percent=%f EventType=%s Name=%s", BeatClockHandle->GetBeatProgressPercent(EQuartzCommandQuantization::Bar), *UEnum::GetValueAsString(EventType), *Name.ToString());

	switch (EventType)
	{
	case EQuartzCommandDelegateSubType::CommandOnQueued:
		_QueueInstructions();
		break;
	case EQuartzCommandDelegateSubType::CommandOnStarted:
		_SubscribeToLoopStart();
		break;
	case EQuartzCommandDelegateSubType::CommandOnFailedToQueue:
		DJLog(Error, "_OnLoopStartCallback EventType=%s Name=%s", *UEnum::GetValueAsString(EventType), *Name.ToString());
		break;
	default:
		break;
	}
}

void UDJBeatInstructionConsumer::_SubscribeToLoopStart()
{
	FQuartzQuantizationBoundary quantBoundary;
	quantBoundary.bFireOnClockStart = true;
	quantBoundary.CountingReferencePoint = EQuarztQuantizationReference::BarRelative;
	quantBoundary.Quantization = EQuartzCommandQuantization::Bar;

	BeatClockHandle->NotifyOnQuantizationBoundary(this, quantBoundary, _OnLoopStartDelegate);
}

void UDJBeatInstructionConsumer::_QueueInstructions()
{
	for (auto instruction : BeatInstructions)
	{
		instruction->_QueueAudioBeforeLoopStart();
	}
}

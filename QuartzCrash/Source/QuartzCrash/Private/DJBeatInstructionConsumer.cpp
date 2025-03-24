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
}

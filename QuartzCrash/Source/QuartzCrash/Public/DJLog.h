#pragma once

#include "Logging/LogMacros.h" 

DECLARE_LOG_CATEGORY_CLASS(DJShooterLog, Log, All);

#define DJLog(Category, String, ...) \
UE_LOG(DJShooterLog, Category, TEXT( "<%#x::" __FUNCTION__ "> - " String), this, ## __VA_ARGS__)

#define DJ_RETURN_IF(Condition)							\
if (Condition) {										\
	DJLog(Error, __FUNCTION__  " - "  #Condition);		\
	return;												\
}

#define DJ_RETURN_FALSE_IF(Condition)					\
if (Condition) {										\
	DJLog(Error, __FUNCTION__  " - "  #Condition);		\
	return false;										\
}
#define DJ_RETURN_NULLPTR_IF(Condition)					\
if (Condition) {										\
	DJLog(Error, __FUNCTION__  " - "  #Condition);		\
	return nullptr;										\
}
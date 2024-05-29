#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ProjectRobbery, Log, All);

#if defined(__GNUC__) || defined(__clang__)
#define CALLINFO (FString(__PRETTY_FUNCTION__) + TEXT(":") + FString::FromInt(__LINE__))
#elif defined(_MSC_VER)
#define CALLINFO (FString(__FUNCTION__) + TEXT(":") + FString::FromInt(__LINE__))
#endif

#define __PRINTF(fmt, ...) FString::Printf(fmt, __VA_ARGS__)
#define __FORMAT(fmt, ...) FString::Format(fmt, __CreateFormatArgs(__VA_ARGS__))

#define __LOG_CALLINFO(__log_level) UE_LOG(ProjectRobbery, __log_level, TEXT("%s"), *CALLINFO)
#define __LOG_FORMATTED(__log_level, __str) UE_LOG(ProjectRobbery, __log_level, TEXT("%s: %s"), *CALLINFO, *__str)

#define LOG_CALLINFO __LOG_CALLINFO(Log)
#define WARNING_CALLINFO __LOG_CALLINFO(Warning)
#define ERROR_CALLINRO __LOG_CALLINFO(Error)

#define TPS_LOG(fmt, ...) __LOG_FORMATTED(Log, __PRINTF(fmt, __VA_ARGS__))
#define TPS_WARNING(fmt, ...) __LOG_FORMATTED(Warning, __PRINTF(fmt, __VA_ARGS__))
#define TPS_ERROR(fmt, ...) __LOG_FORMATTED(Error, __PRINTF(fmt, __VA_ARGS__))

#define FMT_LOG(fmt, ...) __LOG_FORMATTED(Log, __FORMAT(fmt, __VA_ARGS__))
#define FMT_WARNING(fmt, ...) __LOG_FORMATTED(Warning, __FORMAT(fmt, __VA_ARGS__))
#define FMT_ERROR(fmt, ...) __LOG_FORMATTED(Error, __FORMAT(fmt, __VA_ARGS__))

template <typename... Args>
FORCEINLINE TArray<FStringFormatArg> __CreateFormatArgs(Args&&... args)
{
    return TArray<FStringFormatArg>{FStringFormatArg(Forward<Args>(args))...};
}

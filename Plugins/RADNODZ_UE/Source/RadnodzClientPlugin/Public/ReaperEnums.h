// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperEnums.generated.h"

UENUM(BlueprintType)
enum class AZ_FileWriteMode : uint8
{
	Overwrite = 0,

	Numbering = 1,

	Append = 2,

};
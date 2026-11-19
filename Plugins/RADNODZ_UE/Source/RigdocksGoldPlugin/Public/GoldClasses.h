// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReaperClasses.h"
#include "GoldClasses.generated.h"

class XLDocument;
class XLWorksheet;
class GSheetConnection;

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UXLDocument : public UReaperClass
{
public:
	GENERATED_BODY()

	inline static XLDocument* Get(UXLDocument* ptr)
	{
		return ptr ? (XLDocument*)ptr->GetPointer() : nullptr;
	}

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UXLWorksheet : public UReaperClass
{
public:
	GENERATED_BODY()

	inline static XLWorksheet* Get(UXLWorksheet* ptr)
	{
		return ptr ? (XLWorksheet*)ptr->GetPointer() : nullptr;
	}

};

UCLASS(BlueprintType)
class RIGDOCKSGOLDPLUGIN_API UGSheetConnection : public UReaperClass
{
public:
	GENERATED_BODY()

	inline static GSheetConnection* Get(UGSheetConnection* ptr)
	{
		return ptr ? (GSheetConnection*)ptr->GetPointer() : nullptr;
	}

};


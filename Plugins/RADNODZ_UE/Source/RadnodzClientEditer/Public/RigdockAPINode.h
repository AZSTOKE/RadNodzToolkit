// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "K2Node_CallFunction.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "RigdockAPINode.generated.h"

UCLASS()
class RADNODZCLIENTEDITERPLUGIN_API URigdockAPINode : public UK2Node_CallFunction
{
    GENERATED_BODY()

public:

    virtual void ValidateNodeDuringCompilation(FCompilerResultsLog& MessageLog) const override;

    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
};
// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RigdockAPINode.h"
#include "EdGraphSchema_K2.h"
#include "KismetCompiler.h"
#include "RadnodzClient.h"
#include "RadnodzCommonClass.h"

#define LOCTEXT_NAMESPACE "RigdockAPINode"

void URigdockAPINode::ValidateNodeDuringCompilation(FCompilerResultsLog& MessageLog) const
{
    Super::ValidateNodeDuringCompilation(MessageLog);

    UFunction* Function = GetTargetFunction();

    if (!Function)
    {
        return;
    }

    const FString RequiredPinsString = Function->GetMetaData(TEXT("RequiredPins"));
    if (RequiredPinsString.IsEmpty())
    {
        return;
    }

    TArray<FString> RequiredPins;
    RequiredPinsString.ParseIntoArray(RequiredPins, TEXT(","), true);

    for (FString& PinName : RequiredPins)
    {
        PinName = PinName.TrimStartAndEnd();

        UEdGraphPin* Pin = FindPin(*PinName);

        if (!Pin)
        {
            continue;
        }

        const bool bConnected = Pin->LinkedTo.Num() > 0;
        const bool bHasDefaultObject = Pin->DefaultObject != nullptr;

        if (!bConnected && !bHasDefaultObject)
        {
            MessageLog.Error(
                *FString::Printf(TEXT("\"@@ Required pin '%s' is not connected.\""), *PinName),
                this
            );
        }
    }
}

void URigdockAPINode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
    UClass* ActionKey = GetClass();

    if (!ActionRegistrar.IsOpenForRegistration(ActionKey))
    {
        return;
    }

    for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
    {
        UClass* Class = *ClassIt;

        if (!Class)
        {
            continue;
        }

        if (Class->HasAnyClassFlags(CLASS_Deprecated))
        {
            continue;
        }

        if (!Class->IsChildOf(URadnodzCommonClass::StaticClass()))
        {
            continue;
        }

        for (TFieldIterator<UFunction> FuncIt(Class); FuncIt; ++FuncIt)
        {
            UFunction* Function = *FuncIt;

            if (!Function->HasMetaData(TEXT("RequiredPins")))
            {
                continue;
            }

            UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());

            const FName FunctionName = Function->GetFName();

            Spawner->CustomizeNodeDelegate =
                UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateLambda(
                    [Class, FunctionName](UEdGraphNode* NewNode, bool bIsTemplateNode)
                    {
                        URigdockAPINode* Node = CastChecked<URigdockAPINode>(NewNode);

                        Node->FunctionReference.SetExternalMember(FunctionName, Class);
                    });

            ActionRegistrar.AddBlueprintAction(GetClass(), Spawner);
        }
    }
}

#undef LOCTEXT_NAMESPACE
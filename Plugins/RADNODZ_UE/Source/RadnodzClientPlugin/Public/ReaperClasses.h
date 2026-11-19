// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "define.h"
#include <memory>
#include "ReaperClasses.generated.h"

class ReaProject;
class MediaItem;
class MediaItem_Take;
class MediaTrack;

UCLASS()
class RADNODZCLIENTPLUGIN_API UReaperClass : public UObject
{
public:
	GENERATED_BODY()

	UReaperClass();

	void* GetPointer();

	void SetPointer(void* pointer);

	bool IsNull();

protected:
	uint64_t _pointer;

};

UCLASS(BlueprintType)
class RADNODZCLIENTPLUGIN_API UReaProject : public UReaperClass
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	static UReaProject* GetCurrentProject();

	static ReaProject* Get(UReaProject* ptr);

};

UCLASS(BlueprintType)
class RADNODZCLIENTPLUGIN_API UMediaItem : public UReaperClass
{
public:
	GENERATED_BODY()

	static MediaItem* Get(UMediaItem* ptr);};UCLASS(BlueprintType)

class RADNODZCLIENTPLUGIN_API UMediaTrack : public UReaperClass
{
public:
	GENERATED_BODY()

	static MediaTrack* Get(UMediaTrack* ptr);
};UCLASS(BlueprintType)

class RADNODZCLIENTPLUGIN_API UMediaItem_Take : public UReaperClass
{
public:
	GENERATED_BODY()

	static MediaItem_Take* Get(UMediaItem_Take* ptr);
};

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	STRING,
	NUMBER,
	BOOL,
};

UCLASS()
class RADNODZCLIENTPLUGIN_API UVarType : public UObject
{
public:
	GENERATED_BODY()

	UVarType() {};

	void Initialze(VarType& var);

	VarType GetCppObj();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType", meta = (ExpandEnumAsExecs = "ObjectType"))
	void GetType(EObjectType& ObjectType);	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	static UVarType* CreateFromNumber(double value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	static UVarType* CreateFromString(FString value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	static UVarType* CreateFromBool(bool value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	double GetNumber();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	FString GetString();

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	bool GetBool();	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	void SetNumber(double value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	void SetString(FString value);

	UFUNCTION(BlueprintCallable, Category = "RADNODZ|VarType")
	void SetBool(bool value);

private:

	EObjectType _type;

	double numberValue;

	FString stringValue;

	bool boolValue;
};

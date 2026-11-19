// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "ReaperClasses.h"

UReaperClass::UReaperClass(void) :
    Super()
    , _pointer(0)
{
}

void* UReaperClass::GetPointer()
{
	return (void*)(uintptr_t)_pointer;;
}

void UReaperClass::SetPointer(void* pointer)
{
	_pointer = (uint64_t)(uintptr_t)pointer;
}

bool UReaperClass::IsNull()
{
	return _pointer == 0;
}

UReaProject* UReaProject::GetCurrentProject()
{
	return NewObject<UReaProject>();
}

ReaProject* UReaProject::Get(UReaProject* ptr)
{
	ptr = ptr ? ptr : UReaProject::GetCurrentProject();

	return (ReaProject*)ptr->GetPointer();
}

MediaItem* UMediaItem::Get(UMediaItem* ptr)
{
	return ptr ? (MediaItem*)ptr->GetPointer() : nullptr;
}

MediaTrack* UMediaTrack::Get(UMediaTrack* ptr)
{
	return ptr ? (MediaTrack*)ptr->GetPointer() : nullptr;
}

MediaItem_Take* UMediaItem_Take::Get(UMediaItem_Take* ptr)
{
	return ptr ? (MediaItem_Take*)ptr->GetPointer() : nullptr;
}

void UVarType::Initialze(VarType& var)
{
	switch (var.type)
	{
	case ObjectType::STRING:
		_type = EObjectType::STRING;
		stringValue = UTF8_TO_TCHAR(var.stringValue.c_str());
		break;
	case ObjectType::NUMBER:
		_type = EObjectType::NUMBER;
		numberValue = var.numberValue;
		break;
	case ObjectType::BOOL:
		_type = EObjectType::BOOL;
		boolValue = var.boolValue;
		break;
	}
}

VarType UVarType::GetCppObj()
{
	VarType obj;
	switch (_type)
	{
	case EObjectType::STRING:
		obj.type = ObjectType::STRING;
		obj.stringValue = TCHAR_TO_UTF8(*stringValue);
		break;
	case EObjectType::NUMBER:
		obj.type = ObjectType::NUMBER;
		obj.numberValue = numberValue;
		break;
	case EObjectType::BOOL:
		obj.type = ObjectType::BOOL;
		obj.boolValue = boolValue;
		break;
	}

	return obj;

}

void UVarType::GetType(EObjectType& type)
{
	type = _type;

	return;
}

double UVarType::GetNumber()
{
	if (_type == EObjectType::NUMBER)
	{
		return numberValue;
	}
	else
	{
		return 0.0;
	}
}

FString UVarType::GetString()
{
	if (_type == EObjectType::STRING)
	{
		return stringValue;
	}
	else
	{
		return FString("");
	}
}

bool UVarType::GetBool()
{
	if (_type == EObjectType::BOOL)
	{
		return boolValue;
	}
	else
	{
		return false;
	}
}

void UVarType::SetNumber(double value)
{
	_type = EObjectType::NUMBER;
	numberValue = value;
}

void UVarType::SetString(FString value)
{
	_type = EObjectType::STRING;
	stringValue = value;
}

void UVarType::SetBool(bool value)
{
	_type = EObjectType::BOOL;
	boolValue = value;
}

UVarType* UVarType::CreateFromNumber(double value)
{
	UVarType* var = NewObject<UVarType>();
	var->SetNumber(value);

	return var;
}

UVarType* UVarType::CreateFromString(FString value)
{
	UVarType* var = NewObject<UVarType>();
	var->SetString(value);
	return var;
}

UVarType* UVarType::CreateFromBool(bool value)
{
	UVarType* var = NewObject<UVarType>();
	var->SetBool(value);
	return var;

}
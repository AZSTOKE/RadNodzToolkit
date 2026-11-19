// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <string>

class ReaProject;
class MediaItem;
class MediaItem_Take;
class MediaTrack;

enum class ObjectType
{
	STRING,
	NUMBER,
	BOOL,
	POINTER,
};

struct VarType
{
	ObjectType type;

	std::string stringValue;
	double numberValue;
	bool boolValue;
	void* pointerValue;

	VarType() : type(ObjectType::POINTER), stringValue(""), numberValue(0), boolValue(false), pointerValue(nullptr) {}
};
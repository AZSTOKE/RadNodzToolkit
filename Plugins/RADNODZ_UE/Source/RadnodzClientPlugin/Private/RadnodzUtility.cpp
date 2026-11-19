// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#include "RadnodzUtility.h"
#include "RADNODZ_Core.h"
#include <chrono>

FString URadnodzUtility::AZ_GetLocalIP()
{
	return UTF8_TO_TCHAR(RADNODZ_client::Core::AZ_GetLocalIP().c_str());
}

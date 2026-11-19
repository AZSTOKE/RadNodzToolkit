// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include "TcpClient.h"

namespace RADNODZ_client::Core
{

    /****************** License *****************************/

    int AZ_CheckLicense(TcpClient& tcpClient);

    /****************** Device *****************************/

    std::string AZ_GetLocalIP();

}

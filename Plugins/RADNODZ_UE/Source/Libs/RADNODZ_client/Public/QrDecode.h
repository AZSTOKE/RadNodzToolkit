// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.

#pragma once
#include <cstdint>
#include <string>

// QR code reading, for the pairing flow: the client reads the server's QR and
// gets a pairing url out of it (see PairingUrl.h).
//
// The decoder is zxing-cpp (3rd/zxing-cpp, Apache-2.0), built with QR code
// support only. It copes with rotation at any angle, perspective, uneven
// lighting and a code sitting inside a larger screenshot.
//
// This header deliberately exposes no zxing types, so consumers do not need
// zxing on their include path - only the static library on the link line.
//
// Getting the image is left to the caller: this takes 8 bit grayscale, so the
// platform decides how to produce it (UE: ImageWrapper, Android: BitmapFactory,
// a camera: the luminance plane as-is).
namespace RADNODZ_client
{
    // Reason a decode attempt failed. The text is for logging; show the user
    // something localized instead of forwarding this verbatim.
    struct QrDecodeError
    {
        // true when the image was readable but held no QR code, as opposed to
        // the input being unusable (empty / bad size).
        bool notFound = false;

        std::string message;
    };

    /**
     * Read a QR code out of an 8 bit grayscale image.
     *
     * A first pass runs on the image as given. If that finds nothing, a second
     * pass runs on a median filtered copy, which recovers speckled images
     * (screenshots of a noisy signal, a photo of a screen) at the cost of one
     * extra pass. Clean images therefore stay on the fast path.
     *
     * @param gray     luminance, 0 = black, 255 = white
     * @param width    pixels across
     * @param height   pixels down
     * @param stride   bytes per row (pass width when the rows are packed)
     * @param outText  decoded content; byte mode is treated as UTF-8
     * @param outError why it failed
     * @return whether a QR code was decoded
     */
    bool DecodeQrCode(const std::uint8_t* gray, int width, int height, int stride,
        std::string& outText, QrDecodeError& outError);
}

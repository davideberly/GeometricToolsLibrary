// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

#include <Graphics/Texture2.h>
#include <memory>

// The supported texture types are DF_R8G8B8A8_UNORM, DF_R8_UNORM
// and DF_R8G8_UNORM (gray+alpha).

namespace gte
{
    class WICFileIO
    {
    public:
        // Support for loading from PNG. If the load is not successful, the
        // function returns a null object.
        static std::shared_ptr<Texture2> Load(std::string const& filename,
            bool wantMipmaps);

        // Support for saving to PNG format.  The function returns true when
        // successful.
        static bool SaveToPNG(std::string const& filename,
            std::shared_ptr<Texture2> const& texture);
    };
}

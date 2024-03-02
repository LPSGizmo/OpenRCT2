/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include <openrct2-ui/interface/Window.h>
#include <openrct2/common.h>
#include <openrct2/sprites.h>

constexpr uint16_t kLandToolMinimumSize = 1;
constexpr uint16_t kLandToolMaximumSize = 64;
// The highest tool size to have a sprite. Bigger tool sizes simply display a number.
constexpr uint16_t kLandToolMaximumSizeWithSprite = 7;

extern uint16_t gLandToolSize;
extern money64 gLandToolRaiseCost;
extern money64 gLandToolLowerCost;
extern ObjectEntryIndex gLandToolTerrainSurface;
extern ObjectEntryIndex gLandToolTerrainEdge;
extern money64 gWaterToolRaiseCost;
extern money64 gWaterToolLowerCost;

namespace LandTool
{
    uint32_t SizeToSpriteIndex(uint16_t size);
    void ShowSurfaceStyleDropdown(WindowBase* w, Widget* widget, ObjectEntryIndex currentSurfaceType);
    ObjectEntryIndex GetSurfaceStyleFromDropdownIndex(size_t index);
    void ShowEdgeStyleDropdown(WindowBase* w, Widget* widget, ObjectEntryIndex currentEdgeType);
    ObjectEntryIndex GetEdgeStyleFromDropdownIndex(size_t index);
} // namespace LandTool

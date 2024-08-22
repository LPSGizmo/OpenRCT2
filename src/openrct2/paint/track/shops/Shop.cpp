/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../../interface/Viewport.h"
#include "../../../ride/Ride.h"
#include "../../../ride/RideEntry.h"
#include "../../../ride/Track.h"
#include "../../../ride/TrackPaint.h"
#include "../../../sprites.h"
#include "../../../world/Map.h"
#include "../../Boundbox.h"
#include "../../Paint.h"
#include "../../support/WoodenSupports.h"
#include "../../tile_element/Segment.h"
#include "../../track/Segment.h"

using namespace OpenRCT2;

static void PaintShop(
    PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement, SupportType supportType)
{
    bool hasSupports = WoodenASupportsPaintSetupRotated(
        session, WoodenSupportType::Truss, WoodenSupportSubType::NeSw, direction, height,
        GetShopSupportColourScheme(session, trackElement));

    auto rideEntry = ride.GetRideEntry();
    if (rideEntry == nullptr)
        return;

    auto firstCarEntry = &rideEntry->Cars[0];
    if (firstCarEntry == nullptr)
        return;

    CoordsXYZ offset(0, 0, height);
    BoundBoxXYZ bb = { { 2, 2, height }, { 28, 28, trackElement.GetClearanceZ() - trackElement.GetBaseZ() - 3 } };

    auto imageFlags = session.TrackColours.WithoutSecondary();
    auto imageIndex = firstCarEntry->base_image_id + direction;
    if (hasSupports)
    {
        auto foundationImageTemplate = GetShopSupportColourScheme(session, trackElement);
        auto foundationImageIndex = (direction & 1) ? SPR_FLOOR_PLANKS_90_DEG : SPR_FLOOR_PLANKS;
        auto foundationImageId = foundationImageTemplate.WithIndex(foundationImageIndex);
        PaintAddImageAsParent(session, foundationImageId, offset, bb);
        PaintAddImageAsChild(session, imageFlags.WithIndex(imageIndex), offset, bb);
    }
    else
    {
        PaintAddImageAsParent(session, imageFlags.WithIndex(imageIndex), offset, bb);
    }

    PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
    PaintUtilSetGeneralSupportHeight(session, height + 48);

    if (direction == 1 || direction == 2)
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Square, TunnelSubType::Flat);
}

TRACK_PAINT_FUNCTION GetTrackPaintFunctionShop(int32_t trackType)
{
    switch (trackType)
    {
        case TrackElemType::FlatTrack1x1A:
        case TrackElemType::FlatTrack1x1B:
            return PaintShop;
    }
    return nullptr;
}

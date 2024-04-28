/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../entity/EntityRegistry.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/support/WoodenSupports.h"
#include "../Ride.h"
#include "../RideEntry.h"
#include "../Track.h"
#include "../TrackPaint.h"
#include "../Vehicle.h"

static void Paint3dCinemaDome(
    PaintSession& session, const Ride& ride, uint8_t direction, int8_t xOffset, int8_t yOffset, uint16_t height,
    ImageId stationColour)
{
    auto rideEntry = ride.GetRideEntry();
    if (rideEntry == nullptr)
        return;

    if (ride.lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK && !ride.vehicles[0].IsNull())
    {
        session.InteractionType = ViewportInteractionItem::Entity;
        session.CurrentlyDrawnEntity = GetEntity<Vehicle>(ride.vehicles[0]);
    }

    auto imageTemplate = ImageId(0, ride.vehicle_colours[0].Body, ride.vehicle_colours[0].Trim);
    if (stationColour != TrackStationColour)
    {
        imageTemplate = stationColour;
    }

    auto imageId = imageTemplate.WithIndex(rideEntry->Cars[0].base_image_id + direction);
    PaintAddImageAsParent(
        session, imageId, { xOffset, yOffset, height + 3 }, { { xOffset + 16, yOffset + 16, height + 3 }, { 24, 24, 47 } });

    session.CurrentlyDrawnEntity = nullptr;
    session.InteractionType = ViewportInteractionItem::Ride;
}

/**
 * rct2: 0x0076574C
 */
static void Paint3dCinema(
    PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = kTrackMap3x3[direction][trackSequence];

    int32_t edges = edges_3x3[trackSequence];

    auto stationColour = GetStationColourScheme(session, trackElement);
    WoodenASupportsPaintSetupRotated(
        session, WoodenSupportType::Truss, WoodenSupportSubType::NeSw, direction, height, stationColour);

    const StationObject* stationObject = ride.GetStationObject();

    TrackPaintUtilPaintFloor(session, edges, session.TrackColours, height, floorSpritesCork, stationObject);

    TrackPaintUtilPaintFences(
        session, edges, session.MapPosition, trackElement, ride, stationColour, height, fenceSpritesRope,
        session.CurrentRotation);

    switch (trackSequence)
    {
        case 1:
            Paint3dCinemaDome(session, ride, direction, 32, 32, height, stationColour);
            break;
        case 3:
            Paint3dCinemaDome(session, ride, direction, 32, -32, height, stationColour);
            break;
        case 5:
            Paint3dCinemaDome(session, ride, direction, 0, -32, height, stationColour);
            break;
        case 6:
            Paint3dCinemaDome(session, ride, direction, -32, 32, height, stationColour);
            break;
        case 7:
            Paint3dCinemaDome(session, ride, direction, -32, -32, height, stationColour);
            break;
        case 8:
            Paint3dCinemaDome(session, ride, direction, -32, 0, height, stationColour);
            break;
    }

    int32_t cornerSegments = 0;
    switch (trackSequence)
    {
        case 1:
            // top
            cornerSegments = EnumsToFlags(PaintSegment::topCorner, PaintSegment::topLeftSide, PaintSegment::topRightSide);
            break;
        case 3:
            // right
            cornerSegments = EnumsToFlags(PaintSegment::topRightSide, PaintSegment::rightCorner, PaintSegment::bottomRightSide);
            break;
        case 6:
            // left
            cornerSegments = EnumsToFlags(PaintSegment::topLeftSide, PaintSegment::leftCorner, PaintSegment::bottomLeftSide);
            break;
        case 7:
            // bottom
            cornerSegments = EnumsToFlags(
                PaintSegment::bottomLeftSide, PaintSegment::bottomCorner, PaintSegment::bottomRightSide);
            break;
    }

    PaintUtilSetSegmentSupportHeight(session, cornerSegments, height + 2, 0x20);
    PaintUtilSetSegmentSupportHeight(session, kSegmentsAll & ~cornerSegments, 0xFFFF, 0);
    PaintUtilSetGeneralSupportHeight(session, height + 128, 0x20);
}

TRACK_PAINT_FUNCTION GetTrackPaintFunction3dCinema(int32_t trackType)
{
    if (trackType != TrackElemType::FlatTrack3x3)
    {
        return nullptr;
    }

    return Paint3dCinema;
}

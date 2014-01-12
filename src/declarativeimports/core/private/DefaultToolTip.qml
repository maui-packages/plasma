/*
*   Copyright 2013 by Sebastian Kügler <sebas@kde.org>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU Library General Public License as
*   published by the Free Software Foundation; either version 2, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Library General Public License for more details
*
*   You should have received a copy of the GNU Library General Public
*   License along with this program; if not, write to the
*   Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
*/

import QtQuick 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.qtextracomponents 2.0 as QtExtras



Row {
    id: tooltipContentItem

    property Item toolTip

    property int preferredTextWidth: theme.mSize(theme.defaultFont).width * 25
    property int _s: theme.iconSizes.small / 2

    width: childrenRect.width + _s
    height: childrenRect.height
    spacing: _s

    Item {
        id: imageContainer
        visible: toolTip.image || toolTip.icon
        width: Math.max(tooltipImage.width, tooltipIcon.width)
        height: Math.max(tooltipImage.height, tooltipIcon.height)

        Image {
            id: tooltipImage
            source: toolTip ? toolTip.image : ""
        }

        PlasmaCore.IconItem {
            id: tooltipIcon
            width: toolTip.icon != null ? theme.iconSizes.desktop : 0
            height: width
            source: toolTip.icon != null ? toolTip.icon : ""
            anchors {
                leftMargin: width != 0 ? _s : 0
            }
        }
    }

    Column {
        id: mainColumn

        //This instance is purely for metrics
        PlasmaExtras.Heading {
            id: tooltipMaintextPlaceholder
            visible: false
            level: 3
            text: toolTip ? toolTip.mainText : ""
        }
        PlasmaExtras.Heading {
            id: tooltipMaintext
            level: 3
            width: Math.min(tooltipMaintextPlaceholder.width, preferredTextWidth)
            //width: 400
            elide: Text.ElideRight
            text: toolTip ? toolTip.mainText : ""
        }
        PlasmaComponents.Label {
            id: tooltipSubtext
            width: parent.preferredTextWidth
            wrapMode: Text.WordWrap
            text: toolTip ? toolTip.subText : ""
            opacity: 0.5
        }
    }
}



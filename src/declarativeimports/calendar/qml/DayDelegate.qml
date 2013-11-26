/*
 * Copyright 2013 Heena Mahour <heena393@gmail.com>
 * Copyright 2013 Sebastian Kügler <sebas@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.0
import org.kde.plasma.calendar 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as Components
import org.kde.plasma.extras 2.0 as PlasmaExtras


Item {
    id: dayStyle
    width: cellWidth
    height: cellHeight

    //property int borderWidth: root.borderWidth
    property real borderOpacity: root.borderOpacity

    property bool showtop: false
    property bool showright: true
    property bool showleft: false
    property bool showbottom: true

    Rectangle {
        id: todayRect
        x: 0
        y: 0
        //z: -1
        width: parent.width - (borderWidth)
        height: parent.height - (borderWidth)
        //         anchors.fill: parent
        opacity: (isToday(dayNumber+"/"+monthNumber+"/"+yearNumber)) ? 0.8 : 0
        Behavior on opacity { NumberAnimation {} }
        color: theme.highlightColor
        //border.color: isToday(dayNumber+"/"+monthNumber+"/"+yearNumber) ? theme.highlightColor : "transparent"
    }

    Rectangle {
        id: highlightDate
        anchors {
            fill: todayRect
        }
        opacity: {
            if (calendarDays.selectedItem == dayStyle) 0.6
                else if (dateMouse.containsMouse) 0.3
                    else 0
        }
        //visible: dateMouse.containsMouse
        Behavior on opacity { NumberAnimation {} }
        color: theme.viewBackgroundColor
        z: todayRect.z - 1
        //border.color: isToday(dayNumber+"/"+monthNumber+"/"+yearNumber) ? theme.highlightColor : "transparent"
    }

    //     Rectangle {
    //         id: frameTop
    //         height: borderWidth
    //         color: theme.textColor
    //         opacity: borderOpacity
    //         visible: showtop
    //
    //         anchors {
    //             top: parent.top
    //             left: parent.left
    //             right: parent.right
    //         }
    //     }

    Rectangle {
        id: frameRight
        width: borderWidth
        color: theme.textColor
        opacity: borderOpacity
        visible: showright

        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }

    Rectangle {
        id: frameBottom
        height: borderWidth
        color: theme.textColor
        opacity: borderOpacity
        visible: showbottom

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
    }

    //     Rectangle {
    //         id: frameLeft
    //         width: borderWidth
    //         color: theme.textColor
    //         opacity: borderOpacity
    //         visible: showleft
    //
    //         anchors {
    //             top: parent.top
    //             left: parent.left
    //             bottom: parent.bottom
    //         }
    //     }

    Components.Label {
        id: label
        anchors.centerIn: parent
        //anchors.topMargin: 10
        font.pixelSize: Math.max(theme.defaultFont.pixelSize, cellHeight / 3)
        text: dayNumber
        //z: 99
        //font.bold: (containsEventItems)||(containsTodoItems) ? true: false
        opacity: (isPreviousMonth || isNextMonth) ? 0.5: 1.0
    }



    MouseArea {
        id: dateMouse
        anchors.fill: parent
        //z: label.z + 1
        hoverEnabled: true
        onClicked: {
            print("Click");
            print("date change:" + dayNumber + monthNumber)
            var rowNumber = Math.floor ( index / 7)   ;
            week = 1+monthCalendar.weeksModel[rowNumber];
            //date = dayNumber + "/" + monthNumber + "/" + yearNumber
            root.date = model
            calendarDays.selectedItem = dayStyle
        }
    }

    Component.onCompleted: {
        var t = isToday(dayNumber+"/"+monthNumber+"/"+yearNumber)
        if (t) {
            root.date = model;
        }
    }
}
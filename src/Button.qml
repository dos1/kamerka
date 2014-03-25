/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

import QtQuick 1.0;


Rectangle {
    property alias text: textItem.text;
    property alias mouse: mouseArea;
    property alias font: textItem.font;
    property alias tooltip: toolTipText.text;
    id: button;
    width: 100; height: 30;
    border.width: 1;
    radius: 3;
    smooth: true;
    opacity: 0.7;
    z:10;

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#888"; }
        //GradientStop { position: 0.5; color: "black"; }
        GradientStop { position: 1.0; color: "#575757"; }
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
        hoverEnabled: true;
    }

    Text {
        id: textItem;
        anchors.centerIn: parent;
        font.pointSize: 12;
        color: "white";
        Behavior on text {
            SequentialAnimation {
                NumberAnimation { target: textItem; property: "opacity"; to: 0; }
                PropertyAction {}
                NumberAnimation { target: textItem; property: "opacity"; to: 1; }
            }
        }
    }


    Rectangle{
        id: toolTip;
        color: "#fff";
        border.color: "#000";
        border.width: 1;
        x: mouse.mouseX + 10;
        y: mouse.mouseY - toolTip.height;
        z:parent.z + 10
        height: toolTipText.paintedHeight + 4
        width: toolTipText.paintedWidth + 4
        opacity:0;

        Text {
            id: toolTipText;
            anchors.centerIn: parent;
            color: "#000";
            z: parent.z + 10;
            wrapMode: Text.WordWrap;
        }
    }



    states: State {
        name: "down"; when: mouse.containsMouse;
        PropertyChanges {
            target: button;
            opacity: 1;
        }
        PropertyChanges {
            target: toolTip;
            opacity: toolTipText.text ? 1 : 0;
        }
    }

    transitions: Transition {
        from: ""; to: "down"; reversible: true;
        SequentialAnimation {
            NumberAnimation { property: "opacity"; duration: 400; easing.type: Easing.OutQuad; target:button}
        }

    }

}

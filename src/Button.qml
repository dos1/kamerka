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

import Qt 4.7

 Rectangle {
     property alias text: textItem.text
     property alias mouse: mouseArea
     id: button;
     width: 100; height: 30
     border.width: 1
     radius: 5
     smooth: true
     opacity: 0.5;

     gradient: Gradient {
         GradientStop { position: 0.0; color: "darkGray" }
         GradientStop { position: 0.5; color: "black" }
         GradientStop { position: 1.0; color: "darkGray" }
     }

     MouseArea {
         id: mouseArea;
         anchors.fill: parent;
         hoverEnabled: true;
     }

     Text {
         id: textItem
         anchors.centerIn: parent
         font.pointSize: 12
         color: "white"
     }


     states: State {
         name: "down"; when: mouse.containsMouse
         PropertyChanges {
             target: button;
             opacity: 1
         }
     }

     transitions: Transition {
         from: ""; to: "down"; reversible: true
         SequentialAnimation {
             NumberAnimation { property: "opacity"; duration: 400; easing.type: Easing.OutQuad; }
         }

     }

 }

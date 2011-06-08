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
 import Effects 1.0

 Rectangle {
     property int rand: 0;
     property bool cyknij: false;
     property int timercount: 5;
     property bool timeranim: false;
     property bool more: false;
     property bool canvasVisible: false;

     signal takePhoto();
     signal timerCounter(int count);
     signal showDirectory();
     signal showConfiguration();

     id: page
     width: 640; height: 480
     color: "transparent"
     /*Image {
        id: canvas;
        anchors.fill: parent;
        source: "image72.png";
        smooth: true;
     }*/
     Rectangle {
         effect: DropShadow {
           blurRadius: 8
           offset.x: 0
           offset.y: 0
           color: "black"
         }
         id: focia;
         x: -10;
         y: -10;
         z: 100;
         width: page.width+20;
         height: page.height+50;
         color: "white";
         smooth: true;
         opacity: 0;

         states: State {
             name: "down"; when: cyknij == true
             PropertyChanges {
                 target: focia;
                 x: page.width-width-20;
                 y: page.height-250;
                 width: 180;
                 height: 185;
                 rotation: 180*4+(rand-30);
                 opacity: 0;
                 smooth: true;
             }
             PropertyChanges {
                 target: picture;
                 opacity: 1;
             }
         }

         transitions: Transition {
             from: ""; to: "down"; reversible: false
             SequentialAnimation {
                 id: animacja;
                 NumberAnimation { target: picture; property: "opacity"; duration: 500; }
                 NumberAnimation { properties: "width,height,x,y,rotation"; duration: 1000; easing.type: Easing.OutQuad }
                 NumberAnimation { duration: 1000; }
                 NumberAnimation { target: focia; property: "opacity"; duration: 1500; easing.type:Easing.InQuad;}
             }

         }

         Rectangle {
             id: picture;
             x: 10;
             y: 10;
             opacity: 1;
             width: parent.width-20;
             height: parent.height-50;
             color: "black";
             smooth: true;
             /*Image {
                 effect: Blur {
                   blurRadius: 8
                 }
                 opacity: 0.75;
                anchors.fill: parent;
                source: fileName;
                smooth: true;
             }*/
             Image {
                 id: preview;
                anchors.fill: parent;
                source: fileName;
                smooth: true;
             }
         }

     }
     function robFotke() {
       cyknij=false;
       page.takePhoto();

       page.rand=Math.random()*60;
       picture.opacity=0;
       focia.opacity=1;
     }
     function photoTaken() {
         cyknij=true;
     }

     function timerGo() {
       timer.running = true;
       page.timercount=5;
       timerTriggered();
     }
     function timerTriggered() {
        timerCounter(page.timercount);
        if (page.timercount==-1) {
            timer.running=false;
            robFotke();
            page.timeranim=false;
            return;
        }

        page.timeranim=false;
        timerText.text=page.timercount.toString();
        page.timercount--;
        page.timeranim=true;
     }
     function moreOrLess() {
         if (more) { more=false; }
         else { more=true; }
     }
     function showCanvasBackground() {
         canvasVisible=true;
         testTimer.running=true;
     }
     function hideCanvasBackground() {
         canvasVisible=false;
     }

     Rectangle {
         id: canvasBackground;
         color: "#DD000000";
         x: 0;
         y: 0;
         width: parent.width;
         height: parent.height;
         opacity: 0;

         states:
             State {
                 name: "visible"; when: canvasVisible == true
                 PropertyChanges {
                     target: canvasBackground;
                     opacity: 1;
                 }
             }

         transitions:
             Transition {
                 to: "visible"; reversible: true;
                 NumberAnimation { property: "opacity"; duration: 1000; }
             }

     }

     Rectangle {
         id: toolbar;
         y: page.height-(height/2)
         anchors.horizontalCenter: page.horizontalCenter;
         width: 0.75*page.width;
         height: 75;
         color: "#33000000";
         radius: 10;
         opacity: 1;

         Button {
             id: shot;
             anchors.left: parent.left;
             anchors.bottom: parent.verticalCenter;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 4;
             width: (parent.width/3)-8;
             text: i18n("Take a photo");
             mouse.onClicked: robFotke();
         }
         Button {
             id: autoshot;
             anchors.left: shot.right;
             anchors.bottom: parent.verticalCenter;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 2;
             width: (parent.width/3)-8;
             text: i18n("Self-timer");
             mouse.onClicked: timerGo();
         }
         Button {
             id: moreBtn;
             anchors.left: autoshot.right;
             anchors.bottom: parent.verticalCenter;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 2;
             anchors.right:  parent.right;
             anchors.rightMargin:  4;
             text: more ? i18n("Less") : i18n("More");
             mouse.onClicked: moreOrLess();
         }
         Button {
             id: configure;
             anchors.top:  parent.verticalCenter;
             anchors.left: parent.left;
             anchors.bottom: parent.bottom;
             anchors.topMargin: 2;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 4;
             width: (parent.width/2)-8;
             text: i18n("Configure");
             mouse.onClicked: showConfiguration();
         }
         Button {
             id: dolphin;
             anchors.top:  parent.verticalCenter;
             anchors.left: configure.right;
             anchors.bottom: parent.bottom;
             anchors.topMargin: 2;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 2;
             anchors.right:  parent.right;
             anchors.rightMargin:  4;
             text: i18n("Open directory");
             mouse.onClicked: showDirectory();
         }


         states: [
             State {
                 name: "down"; when: timer.running == true
                 PropertyChanges {
                     target: toolbar;
                     y: page.height;
                 }
             },
             State {
                 name: "hidden"; when: canvasVisible == true
                 PropertyChanges {
                     target: toolbar;
                     y: page.height;
                     color: "#AA000000";
                 }
             },
             State {
                 name: "more"; when: more == true
                 PropertyChanges {
                     target: toolbar;
                     y: page.height-height-4;
                     color: "#AA000000";
                 }
             }
         ]

         transitions: [
             Transition {
                 to: "down"; reversible: false;
                 SequentialAnimation {
                     NumberAnimation { property: "y"; duration: 500; easing.type: Easing.OutQuad; }
                 }
             },
             Transition {
                 to: "hidden"; reversible: true;
                 SequentialAnimation {
                     NumberAnimation { property: "y"; duration: 500; easing.type: Easing.InOutBack; }
                 }
             },
             Transition {
                 from: ""; to: "more"; reversible: true;
                 ParallelAnimation {
                     NumberAnimation { property: "y"; duration: 500; easing.type: Easing.InOutBack; }
                     ColorAnimation { property: "color"; duration: 500; }
                 }
             }
         ]

     }

     Timer { //REMOVE ME
         id: testTimer;
         interval: 5000; running: false; repeat: false;
         onTriggered: hideCanvasBackground()
     }

     Timer {
         id: timer;
         interval: 1000; running: false; repeat: true;
         onTriggered: timerTriggered()
     }

     Text {
         id: timerText
         anchors.centerIn: parent
         font.pointSize: 200
         color: "white"
         opacity: 0;
         text: "5";
         visible: false;

         effect: DropShadow {
           blurRadius: 8
           offset.x: 0
           offset.y: 0
           color: "black";
         }

         states: State {
             name: "down"; when: page.timeranim == true
             PropertyChanges {
                 target: timerText;
                 opacity: 1;
                 visible: true;
             }
         }

         transitions: Transition {
             from: ""; to: "down"; reversible: false
             SequentialAnimation {
                 NumberAnimation { property: "opacity"; duration: 500; easing.type: Easing.OutQuad; }
             }

         }
     }

 }

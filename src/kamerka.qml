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

import QtQuick 2.0;
import QtGraphicalEffects 1.0;


Rectangle {
    property int rand: 0;
    property bool take: true;
    property int selftimer;
    property int timercount: 5;
    property bool timeranim: false;
    property bool more: false;
    property bool canvasVisible: false;
    property bool recording: false;
    property bool effects: false;
    property int currentEffect: 0;
    property int burstPhotoNumber;
    property int burstPhotosDone: 0;
    property int delayBetweenPhotosBurst;

    signal takePhoto();
    signal timerCounter(int count);
    signal showDirectory();
    signal showConfiguration();
    signal applyEffect(int efx);

    id: page;
    color: "transparent";

    // Load the "FontAwesome" font for the monochrome icons.
    FontLoader {
        source: "fonts/fontawesome-webfont.ttf";
        id: fontAwesome;
    }


    Rectangle {
        id: photo;
        x: -10;
        y: -10;
        z: 100;
        width: page.width+20;
        height: page.height+50;
        color: "white";
        smooth: true;
        opacity: 0;

        states: State {
            name: "down"; when: take == true;
            PropertyChanges {
                target: photo;
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
            from: ""; to: "down"; reversible: false;
            SequentialAnimation {
                id: animacja;
                NumberAnimation { target: picture; property: "opacity"; duration: 500; }
                NumberAnimation { properties: "width,height,x,y,rotation"; duration: 1000; easing.type: Easing.OutQuad; }
                NumberAnimation { duration: 1000; }
                NumberAnimation { target: photo; property: "opacity"; duration: 1500; easing.type:Easing.InQuad; }
            }

        }

        Rectangle {
            id: picture;
            x: 10;
            y: 10;
            opacity: 1;
            width: parent.width-20;
            height: parent.height-50;
            color: "white";
            smooth: true;
            Image {
                id: preview;
                anchors.fill: parent;
                fillMode: Image.PreserveAspectFit;
                source: fileName;
                smooth: true;
                asynchronous: true;
                cache: false;
            }
        }

    }
    DropShadow {
        radius: 8;
        color: "black";
        source: photo;
    }

    function setEffect(id) {
        page.currentEffect = id;
        page.applyEffect(id);
    }

    function doPhoto() {
        take=false;
        page.takePhoto();

        page.rand=Math.random()*60;
        picture.opacity=0;
        photo.opacity=1;
    }
    function photoTaken() {
        take=true;
    }

    function timerGo() {
        timer.running = true;
        page.timercount=page.selftimer;
        timerTriggered();
    }
    function timerTriggered() {
        timerCounter(page.timercount);
        if (page.timercount==-1) {
            timer.running=false;
            doPhoto();
            page.timeranim=false;
            return;
        }

        page.timeranim=false;
        timerText.text=page.timercount.toString();
        page.timercount--;
        page.timeranim=true;
    }

    function doBurstPhoto(){
        burstshot.active = true;
        if(page.burstPhotosDone < page.burstPhotoNumber){
            doPhoto();
            page.burstPhotosDone ++;
        }
        else{
            burstPhotosTimer.stop();
            page.burstPhotosDone = 0;
            burstshot.active = false;
        }
    }

    function showCanvasBackground() {
        canvasVisible=true;
    }
    function hideCanvasBackground() {
        canvasVisible=false;
    }
    function toggleEffects(){
        if (effects) { effects=false; }
        else { effects=true; }
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
            name: "visible"; when: canvasVisible == true;
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
        y: page.height-height;
        anchors.horizontalCenter: page.horizontalCenter;
        width: height*6-24;
        height: Math.max(Math.min(144, Math.min(page.height,page.width)/6),26);
        opacity: 1;
        color: "transparent";

        Button {
            id: shot;
            anchors.left: parent.left;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 4;
            width: parent.height-8;
            height: parent.height-8;
            tooltip: i18n("Take a photo");
            font.pixelSize: width/2;
            font.family: fontAwesome.name;
            text: "\uF083";
            mouse.onClicked: doPhoto();
            z: 11;
        }
        Button {
            id: burstshot;
            anchors.left: shot.right;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 4;
            anchors.leftMargin: 4;
            width: parent.height-8;
            height: parent.height-8;
            tooltip: i18n("Burst mode");
            font.pixelSize: width/2;
            font.family: fontAwesome.name;
            text: "\uF00a";
            mouse.onClicked: burstPhotosTimer.start();
            z: 10;
            Timer {
                id: burstPhotosTimer;
                interval: page.delayBetweenPhotosBurst * 1000;
                running: false;
                repeat: true;
                triggeredOnStart: true;
                onTriggered: doBurstPhoto();
            }
        }
        Button {
            id: autoshot;
            anchors.left: burstshot.right;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 4;
            anchors.leftMargin: 4;
            width: parent.height-8;
            height: parent.height-8;
            tooltip: i18n("Self-timer");
            font.pixelSize: width/2;
            font.family: fontAwesome.name;
            text: "\uf017";
            mouse.onClicked: timerGo();
            z: 9;
        }

        Button {
            id: effectsBtn;
            anchors.left: autoshot.right;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 4;
            anchors.leftMargin: 4;
            width: parent.height-8;
            height: parent.height-8;
            tooltip: effects ? i18n("Hide effects") : i18n("Show effects");
            font.pixelSize: width/2;
            font.family: fontAwesome.name;
            text: "\uf0d0";
            mouse.onClicked: toggleEffects();
            active: effects;
            z: 8;
        }
        Button {
            id: dolphin;
            anchors.left: effectsBtn.right;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 4;
            anchors.leftMargin: 4;
            width: parent.height-8;
            height: parent.height-8;
            tooltip: i18n("Open directory");
            font.family: fontAwesome.name;
            text: "\uf115";
            font.pixelSize: width/2;
            mouse.onClicked: showDirectory();
            z: 7;
        }
        Button {
            id: configure;
            anchors.left: dolphin.right;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: 4;
            anchors.leftMargin: 4;
            width: parent.height-8;
            height: parent.height-8;
            tooltip: i18n("Configure");
            font.pixelSize: width/2;
            font.family: fontAwesome.name;
            text: "\uf013";
            mouse.onClicked: showConfiguration();
            z: 6;
        }

        states: [
            State {
                name: "down"; when: timer.running == true;
                PropertyChanges {
                    target: toolbar;
                    y: page.height;
                }
            },
            State {
                name: "hidden"; when: canvasVisible == true; extend: "more";
                PropertyChanges {
                    target: toolbar;
                    y: page.height;
                }
            },
            State {
                name: "more"; when: more == true;
                PropertyChanges {
                    target: toolbar;
                    y: page.height-height-4;
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

    Timer {
        id: timer;
        interval: 1000; running: false; repeat: true;
        onTriggered: timerTriggered();
    }

    Text {
        id: timerText;
        anchors.centerIn: parent;
        font.pixelSize: 200;
        color: "white";
        opacity: 0;
        text: "5";
        visible: false;

        states: State {
            name: "down"; when: page.timeranim == true;
            PropertyChanges {
                target: timerText;
                opacity: 1;
                visible: true;
            }
        }

        transitions: Transition {
            from: ""; to: "down"; reversible: false;
            SequentialAnimation {
                NumberAnimation { property: "opacity"; duration: 500; easing.type: Easing.OutQuad; }
            }

        }
    }
    DropShadow {
        radius: 8;
        color: "black";
        source: timerText;
    }

    Text {
        id: recText;
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.topMargin: 25;
        anchors.rightMargin: 25;
        font.pixelSize: 40;
        font.bold: true;
        font.family: "Sans";
        color: "red";
        opacity: 0;
        text: "REC";

        states: State {
            name: "down"; when: page.recording == true;
            PropertyChanges {
                target: recText;
                opacity: 1;
            }
        }

        transitions: Transition {
            from: ""; to: "down"; reversible: true;
            SequentialAnimation {
                NumberAnimation { property: "opacity"; duration: 500; easing.type: Easing.OutQuad; }
            }

        }
    }

    Text {
        id: recCircle;
        anchors.right: recText.left;
        anchors.top: parent.top;
        anchors.topMargin: 20;
        anchors.rightMargin: 10;
        font.pixelSize: 40;
        font.bold: true;
        font.family: "Sans";
        color: "red";
        opacity: 1;
        text: "●";
        visible: true;

        states: State {
            name: "down"; when: page.recording == false;
            PropertyChanges {
                target: recCircle;
                opacity: 0;
                visible: false;
            }
            StateChangeScript {
                name: "playRecAnim";
                script: animateOpacity.start();
            }
        }

        SequentialAnimation {
            id: animateOpacity;
            PauseAnimation { duration: 1000; }
            NumberAnimation {
                target: recCircle;
                properties: "opacity";
                from: 1.0;
                to: 0.8;
                duration: 4000;
            }
            PauseAnimation { duration: 1000; }
            NumberAnimation {
                target: recCircle;
                properties: "opacity";
                from: 0.8;
                to: 1.0;
                duration: 4000;
            }
            loops: Animation.Infinite;
        }
    }

    Rectangle {
        id: effectHolder;
        x: 0 - effectHolder.width;
        y: 20;
        height: page.height-40;
        width: 140;
        color: "transparent";
        radius: 6;

        MouseArea {
            id: mouseArea;
            anchors.fill: parent;
            hoverEnabled: true;
        }

        Button {
            id: effect_none;
            x: 0;
            anchors.top: parent.top;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("No Effect");
            mouse.onClicked: setEffect(0);
            active: currentEffect === 0;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }

        Button {
            id: effect_grey;
            x: 0;
            anchors.top: effect_none.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Grey");
            mouse.onClicked: setEffect(1);
            active: currentEffect === 1;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }

        Button {
            id: effect_invert;
            x: 0;
            anchors.top: effect_grey.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Invert");
            mouse.onClicked: setEffect(2);
            active: currentEffect === 2;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }

        Button {
            id: effect_equalize;
            x: 0;
            anchors.top: effect_invert.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Equalize");
            mouse.onClicked: setEffect(3);
            active: currentEffect === 3;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }

        Button {
            id: effect_smurf;
            x: 0;
            anchors.top: effect_equalize.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Smurf");
            mouse.onClicked: setEffect(4);
            active: currentEffect === 4;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_implode;
            x: 0;
            anchors.top: effect_smurf.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Implode");
            mouse.onClicked: setEffect(5);
            active: currentEffect === 5;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_explode;
            x: 0;
            anchors.top: effect_implode.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Explode");
            mouse.onClicked: setEffect(6);
            active: currentEffect === 6;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_charcoal;
            x: 0;
            anchors.top: effect_explode.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Charcoal");
            mouse.onClicked: setEffect(7);
            active: currentEffect === 7;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_edge;
            x: 0;
            anchors.top: effect_charcoal.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Edge");
            mouse.onClicked: setEffect(8);
            active: currentEffect === 8;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_emboss;
            x: 0;
            anchors.top: effect_edge.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Emboss");
            mouse.onClicked: setEffect(9);
            active: currentEffect === 9;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_swirl;
            x: 0;
            anchors.top: effect_emboss.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Swirl");
            mouse.onClicked: setEffect(10);
            active: currentEffect === 10;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_oilpaint;
            x: 0;
            anchors.top: effect_swirl.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Oil Paint");
            mouse.onClicked: setEffect(11);
            active: currentEffect === 11;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }
        Button {
            id: effect_wave;
            x: 0;
            anchors.top: effect_oilpaint.bottom;
            anchors.topMargin: 4;
            anchors.left: parent.left;
            anchors.leftMargin: 5;
            anchors.right: parent.right;
            anchors.rightMargin: 5;
            text: i18n("Wave");
            mouse.onClicked: setEffect(12);
            active: currentEffect === 12;
            font.weight: active ? Font.Bold : Font.Normal;
            font.pixelSize: 14;
        }


        states: [
            State {
                name: "left"; when: timer.running == true;
                PropertyChanges {
                    target: effectHolder;
                    x: 0 - effectHolder.width;
                }
            },
            State {
                name: "hidden"; when: canvasVisible == true; extend: "more";
                PropertyChanges {
                    target: effectHolder;
                    x: 0 - effectHolder.width;
                }
            },
            State {
                name: "effects"; when: effects == true;
                PropertyChanges {
                    target: effectHolder;
                    x: 4;
                }
            }
        ]

        transitions: [
            Transition {
                to: "left"; reversible: false;
                SequentialAnimation {
                    NumberAnimation { property: "x"; duration: 500; easing.type: Easing.OutQuad; }
                }
            },
            Transition {
                to: "hidden"; reversible: true;
                SequentialAnimation {
                    NumberAnimation { property: "x"; duration: 500; easing.type: Easing.InOutBack; }
                }
            },
            Transition {
                from: ""; to: "effects"; reversible: true;
                ParallelAnimation {
                    NumberAnimation { property: "x"; duration: 500; easing.type: Easing.InOutBack; }
                }
            }
        ]
    }

}

 import QtQuick 1.0
 import Effects 1.0

 Rectangle {
     property int randdd: 0;
     //property bool cyknij: false;
     property bool cyknij: false;
     property int timercount: 5;
     property bool timeranim: false;
     //property string fileName: "";
     signal photoTaken();
     signal timerCounter(int count);
     signal showDirectory();
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
         /*effect: DropShadow {
           blurRadius: 8
           offset.x: 0
           offset.y: 0
           color: "black"
         }*/
         id: focia;
         x: -10;
         y: -10;
         z: 100;
         width: page.width+20;
         height: page.height+50;
         color: "white";
         //rotation: 0;
         smooth: true;
         opacity: 0;

         states: State {
             name: "down"; when: cyknij == true
             PropertyChanges {
                 target: focia;
                 x: page.width-width-20;
                 y: page.height-250;
                 //rotation: 180*3;
                 width:180;
                 height: 185;
                 rotation: 180*4+(randdd-30);
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
                source: "image72.png";
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
       page.photoTaken();

       page.randdd=Math.random()*60;
       picture.opacity=0;
       focia.opacity=1;
        // cyknij=true;
     }
     function fotkaZrobiona() {
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

     /*MouseArea {
       id: mouseArea;
       anchors.fill: parent;
       onClicked:robFotke();
     }*/

     Rectangle {
         id: toolbar;
         y: page.height-(height/2)
         anchors.horizontalCenter: page.horizontalCenter;
         width: 0.75*page.width;
         height: 75;
         color: "#33000000";
         radius: 10;
         opacity: 1;
         MouseArea {
           id: mouseArea2;
           anchors.fill: parent;
           //hoverEnabled: true;
           //preventStealing: true;
         }

         Button {
             id: shot;
             anchors.left: parent.left;
             anchors.bottom: parent.verticalCenter;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 4;
             width: (parent.width/3)-8;
             //anchors.right:  parent.horizontalCenter;
             //anchors.rightMargin:  2;
             text: "Zrób zdjęcie";
             mouse.onClicked:robFotke();
         }
         Button {
             id: autoshot;
             anchors.left: shot.right;
             anchors.bottom: parent.verticalCenter;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 2;
             width: (parent.width/3)-8;
             //anchors.right:  parent.right;
             //anchors.rightMargin:  4;
             text: "Samowyzwalacz";
             mouse.onClicked:timerGo();
         }
         Button {
             id: dolphin;
             anchors.left: autoshot.right;
             anchors.bottom: parent.verticalCenter;
             anchors.bottomMargin: 4;
             anchors.leftMargin: 2;
             //width: parent.width/3;
             anchors.right:  parent.right;
             anchors.rightMargin:  4;
             text: "Otwórz folder";
             mouse.onClicked:showDirectory();
         }

         states: State {
             name: "down"; when: timer.running == true
             PropertyChanges {
                 target: toolbar;
                 y: page.height;
             }
         }

         transitions: Transition {
             from: ""; to: "down"; reversible: false
             SequentialAnimation {
                 NumberAnimation { property: "y"; duration: 500; easing.type: Easing.OutQuad; }
             }

         }


     }

     Timer {
         id: timer;
         interval: 1000; running: false; repeat: true
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

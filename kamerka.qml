 import QtQuick 1.0
 import Effects 1.0

 Rectangle {
     property int randdd: 0;
     property bool cyknij: false;
     id: page
     width: 640; height: 480
     color: "transparent"
     Image {
        anchors.fill: parent;
        source: "image72.png";
        smooth: true;
     }
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

         states: State {
             name: "down"; when: cyknij == true
             PropertyChanges {
                 target: focia;
                 x: page.width-width-20;
                 y: page.height-250;
                 //rotation: 180*3;
                 width:200;
                 height: 180;
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
             height: parent.height-40;
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
                anchors.fill: parent;
                source: "image72.png";
                smooth: true;
             }
         }

     }
function robFotke() {
         page.cyknij=false;
      page.randdd=Math.random()*60;
      picture.opacity=0;
      page.cyknij=true;
     }
     MouseArea { id: mouseArea; anchors.fill: parent; onClicked:robFotke(); }
     Text {
         id: helloText
         visible: false;
         text: "Teścik \n większy"
         y: 20 //(page.height/2)-(height/2)
         anchors.horizontalCenter: page.horizontalCenter
         font.pointSize: 24; font.bold: true


         states: State {
             name: "down"; when: mouseArea.pressed == true
             PropertyChanges { target: helloText; y: page.height-height; rotation: 180; color: "red" }
         }

         transitions: [ Transition {
             from: ""; to: "down"; reversible: false
             ParallelAnimation {
                 NumberAnimation { properties: "y,rotation"; duration: 1000; easing.type: Easing.OutBounce }
                 ColorAnimation { duration: 500 }
             }
         }, Transition {
             from: "down"; to: ""; reversible: false
             SequentialAnimation {
                 NumberAnimation { properties: "y,rotation"; duration: 500; easing.type: Easing.InOutQuad }
                 ColorAnimation { duration: 500 }
             }
         }]

     }

 }

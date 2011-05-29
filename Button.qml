import QtQuick 1.0

 Rectangle {
     property alias text: textItem.text
     property alias mouse: mouseArea
     id: button;
     width: 100; height: 30
     border.width: 1
     radius: 5
     smooth: true
     opacity: 0.3;

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

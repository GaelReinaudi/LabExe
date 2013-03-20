import QtQuick 1.0

import GSequence 1.0 as Sequence

 Rectangle {
     width: 3*seq.length
     height: 200
     ColorAnimation on color { to: "green"; duration: 50 }

     MouseArea {
         anchors.fill: parent
         onClicked: {
             parent.color = "green"
         }
     }

     Sequence {

     }

     Column {
         id: channels
         Repeater {
             model: seq.channelCount
             Rectangle {
                 width: 100.5; height: 25.0
                 radius: 2
                 //border.width: 1
                 color: "blue"
                 MouseArea {
                     anchors.fill: parent
                     onClicked: {
                         parent.color = "red"
                     }
                     hoverEnabled: true
                     onEntered: parent.color = "red"
                     onExited: parent.color = "blue"
                 }
                 Text {
                     anchors.centerIn: parent
                     text: "hello"
                     font.pointSize: 12
                     scale: 1.0
                 }
                 scale: 1
             }

         }
     }
 }

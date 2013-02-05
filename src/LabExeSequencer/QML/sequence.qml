import QtQuick 1.0

//import GSequence 1.0 as Sequence

 Rectangle {
     width: 2 * channels.width//seq.Length()
     height: 2000
     ColorAnimation on color { to: "green"; duration: 500 }

     MouseArea {
         anchors.fill: parent
         onClicked: {
             parent.color = "green"
         }
     }

     Column {
         id: channels
         Repeater {
             model: 200//seq.numChannels
             Rectangle {
                 width: 100.5; height: 5.0
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
                     scale: .2
                 }
                 scale: 1
             }

         }
     }

 }

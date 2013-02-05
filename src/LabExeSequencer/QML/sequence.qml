import QtQuick 1.0

//import GSequence 1.0 as Sequence

 Rectangle {
     width: chanels.width//seq.Length()
     height: 200
     ColorAnimation on color { to: "green"; duration: 1000 }

     MouseArea {
         anchors.fill: parent
         onClicked: {
             parent.color = "green"
         }
     }

     Column {
         id: chanels
         Repeater {
             model: seq.numChannels
             Rectangle {
                 width: 1000; height: 40
                 border.width: 1
                 color: "blue"
                 MouseArea {
                     anchors.fill: parent
                     onClicked: {
                         parent.color = "red"
                     }
                 }
             }
         }
     }

 }

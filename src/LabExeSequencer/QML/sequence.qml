import QtQuick 1.0

//import GSequence 1.0 as Sequence

 Rectangle {
     width: chanels.width//seq.Length()
     height: 200
     ColorAnimation on color { to: "yellow"; duration: 1000 }

     Column {
         id: chanels
         Repeater {
             model: 3//seq.NumChanels()
             Rectangle {
                 width: 1000; height: 40
                 border.width: 1
                 color: "blue"
             }
         }
     }

     MouseArea {
         anchors.fill: parent
         onClicked: {
             parent.color = "green"
         }
     }
 }

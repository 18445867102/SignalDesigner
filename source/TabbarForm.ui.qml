import QtQuick 2.9

Item {
    width: 24
    height: 400

    Column{
        x:0; y:0
        spacing: 0

        Rectangle{
            id: rectangle
            width: 24
            height: 40
            border.color: "lightgrey"
            color: "red"

            Text {
                id: name
                x: -8
                y: 8
                width: 40
                height: 24
                text: qsTr("next")
                renderType: Text.NativeRendering
                font.family: "Verdana"
                font.pointSize: 21
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                rotation: 90
            }
        }

        Rectangle{
            width: 24
            height: 40
            color: "red"
        }
        Rectangle{
            width: 24
            height: 40
            color: "YellowGreen"
        }
    }
}

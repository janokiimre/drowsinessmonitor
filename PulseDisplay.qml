import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtCharts 2.15

ColumnLayout {


    width: 120
    anchors.topMargin: 10
    anchors.rightMargin: 10

    Rectangle{
        id: spoContainer

        Layout.fillWidth: true
        Layout.preferredHeight: 50
        border.width: 1
        border.color: "black"
        color: "#120000FF"

        Text{
            id: spoLabel
            text: "SPO2: "
            anchors {
                left: parent.left
                top: parent.top
                margins: 5
            }
        }

        Text{
            text: controller.currentSPO2
            verticalAlignment: Text.AlignVCenter
            font {
                pixelSize: 40
            }
            anchors {
                bottom: parent.bottom
                right: parent.right
                rightMargin: 10

            }
        }
    }

    Rectangle{
        id: pulseContainer
        Layout.preferredHeight: 50
        Layout.fillWidth: true
        border.width: 1
        border.color: "black"
        color: "#120000FF"

        Text{
            id: pulseLabel
            text: "Pulse: "
            anchors {
                left: parent.left
                top: parent.top
                margins: 5
            }
        }

        Text{
            text: controller.currentPulse
            verticalAlignment: Text.AlignVCenter
            font {
                pixelSize: 40
            }
            anchors {
                bottom: pulseContainer.bottom
                right: pulseContainer.right
                rightMargin: 10
            }
        }
    }


}

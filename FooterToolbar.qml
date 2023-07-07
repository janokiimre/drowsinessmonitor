import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtCharts 2.15

ToolBar{
    id:footer

    width: app.width
    anchors.bottom: app.bottom
    implicitHeight: 50
    anchors.bottomMargin: 20


    background: Rectangle { // set a blue rectangular background to the ToolBar
                color: "#00000000"
              //  radius: 10
             //   opacity: 0.7
            }

    RowLayout {
        width: parent.width
        Rectangle {
            color: controller.leftEyeClosed ? "#120000FF":"#00000000"
            height: footer.height
            width: leftEye.paintedWidth + 10
            radius: 10

            Text{
               id: leftEye
               visible: controller.leftEyeClosed ? true:false
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
               anchors.fill: parent
               anchors.centerIn: parent
               font.family: "Arial"
               text: " Left eye closed"
               font.bold: true

           }


        }

        Rectangle {
            color: controller.mouthOpened ? "#120000FF":"#00000000"
            height: footer.height
            width: mouth.paintedWidth + 10
            radius: 10

            Text{
                id: mouth
                visible: controller.mouthOpened ? true:false
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
                font.family: "Arial"
                font.bold: true
                text: "Mouth opened"
            }
            Layout.alignment: Qt.AlignHCenter

        }


        Rectangle {
            color: controller.rightEyeClosed ? "#120000FF":"#00000000"
            height: footer.height
            width: rightEye.paintedWidth + 10
            radius: 10



            Text{
                id: rightEye
                visible: controller.rightEyeClosed ? true: false;
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
                font.family: "Arial"
                font.bold: true
                text: " Right eye closed"

            }

            Layout.alignment: Qt.AlignRight
        }
    }

}

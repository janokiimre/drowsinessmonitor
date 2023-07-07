import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Styles 1.4

ToolBar {

    background: Rectangle {
                  color: "#023B74"
                //  radius: 10
              }

  property bool landmarkWorking: false;

  implicitHeight: 40

  RowLayout{
      Layout.fillWidth: true
      anchors.fill: parent
      RowLayout {
            id: cameraLayout
            property bool isExtended: false
            property bool isRecording: false
            Layout.fillHeight: true
            layoutDirection: Qt.LeftToRight
            spacing:2

            onIsExtendedChanged: {
                console.log("The cameraLayout isExtended value changed to: " + isExtended)
            }

            ToolButton {
                id:camera_button
                icon.source: "qrc:/Images/camera.png"
                icon.color: "white"
                background: Rectangle {
                    color: "#00000000"
                }

                onClicked: {
                    cameraLayout.isExtended = !cameraLayout.isExtended
                    if (cameraLayout.isExtended){
                        icon.source = "qrc:/Images/no_camera.png";
                        viewStack.push(page1);
                      //  viewStack.state = "videopage";
                        controller.startCameraSignal();
                       // camera.start();
                    }
                    else {
                        icon.source = "qrc:/Images/camera.png";
                        viewStack.pop();
                        viewStack.state = "main";
                        controller.stopAll();
                      //  camera.stop();
                    }
                }
            }

            ToolButton {
                visible:cameraLayout.isExtended
                id:camera_button2
                icon.source: "qrc:/Images/camera.png"
                icon.color: "white"
               // anchors.left: camera_button.right
                Layout.alignment: Qt.AlignLeft
                onClicked:{
                   // camera.capture();
                }
                background: Rectangle {
                    color: "#00000000"
                }
            }

            ToolButton {
                visible:cameraLayout.isExtended
                id:camera_button3
                icon.source: cameraLayout.isRecording ? "qrc:/Images/no_video.png" : "qrc:/Images/record_red.png"
                icon.color: cameraLayout.isRecording ? "white" : "red"
                Layout.alignment: Qt.AlignLeft
                background: Rectangle {
                    color: "#00000000"
                }
                onClicked:{
                    if(cameraLayout.isRecording){
                        cameraLayout.isRecording = false;
                      //  camera.stopRecording();
                    }
                    else {
                     //   cameraLayout.isRecording = camera.startRecording();
                    }
                }
            }

            ToolButton {
                visible:cameraLayout.isExtended
                id:landmark
                icon.source: "qrc:/Images/landmark.png"
                icon.color: "white"
                Layout.alignment: Qt.AlignLeft
                background: Rectangle {
                    color: "#00000000"
                }
                onClicked:{
                    landmarkWorking = true;
                    controller.startLandmarkSignal();
                }
            }

            ToolButton {
                visible:cameraLayout.isExtended
                id:settings
                icon.source: "qrc:/Images/settings.png"
                icon.color: "white"
                background: Rectangle {
                    color: "#00000000"
                }
                Layout.alignment: Qt.AlignLeft

            }
        }

        RowLayout{
            Layout.alignment: Qt.AlignRight

            id: pulseLayout
            property bool isExtended: false
            Layout.fillHeight: true
            spacing: 2

            onIsExtendedChanged: {
                console.log("The pulseLayout isExtended value changed to: " + isExtended)
            }

            ToolButton {
                visible: pulseLayout.isExtended
                id:pulse_button2
                background: Rectangle {
                    color: "#00000000"
                }
                icon.source: "qrc:/Images/activity.png"
                icon.color: "white"
            }

            ToolButton {
                id:pulse_button
                icon.source: "qrc:/Images/pulse.png"
                background: Rectangle {
                    color: "#00000000"
                }
                icon.color: "white"
                onClicked: {
                    pulseLayout.isExtended = !pulseLayout.isExtended
                    if (pulseLayout.isExtended){
                        icon.color = "red";
                        controller.startPPGSignal();
                        app.pulseDisplayVisible_parent = true;
                    }
                    else{
                        icon.color = "white";
                        app.pulseDisplayVisible_parent = false;
                    }
                }
            }



        }
  }



}

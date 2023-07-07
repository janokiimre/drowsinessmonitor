import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtCharts 2.15

ApplicationWindow {
    id: app
    visible: true

    height: 900
    width: 900

    FontLoader {
        id: webFont
        source: "qrc:/font/Technology-Bold.ttf"
    }

    Image {
        id: image1
        height: parent.height
        fillMode: Image.PreserveAspectFit
        anchors.bottom: parent.bottom
        horizontalAlignment: Image.AlignLeft
        source: "qrc:/Images/sztaki_new.jpg"
    }

    menuBar: MainToolBar {
        id: toolbar
    }

    Timer{
        id: calib_timer
        interval:5000
        running:false
        repeat:false
        onTriggered:{
            switch(viewStack.state){
                case "normal":{
                    viewStack.replace(page3);
                    viewStack.state = "main";
                    controller.currentState = "main";
                    break;
                }
                case "fully_opened":{
                    viewStack.replace(page4);
                    viewStack.state = "main";
                    controller.currentState = "main";
                    break;
                }
                case "fully_closed":{
                    viewStack.replace(page5);
                    viewStack.state = "main";
                    controller.currentState = "main";
                    break;
                }

            }

        }

    }

    StackView {
        id: viewStack
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        state: "main"
        onStateChanged: {
            console.log("stackview state changed: ", viewStack.state);
            console.log("controller state changed: ", controller.currentState);
        }

        initialItem: image1

        states: [
            State{
                name: "main"
            },

            State {
                name: "normal"
                StateChangeScript {
                    script: {
                        viewStack.replace(videoPage);
                        controller.startLandmarkSignal();
                    }
                }
           },

            State {
                name: "fully_opened"
                StateChangeScript {
                    script: {
                        viewStack.replace(videoPage);
                    }
                }
           },

            State {
                name: "fully_closed"
                StateChangeScript {
                    script: {
                        viewStack.replace(videoPage);
                    }
                }
           },

            State {
                name: "videopage"
                StateChangeScript {
                    script: {
                        viewStack.replace(videoPage);
                    }
                }
           }
        ]

        Timer{
            id: first_timer
            interval: 13000 // Timer interval in milliseconds (1 second)
            running: false
            repeat: false // Repeat the timer
            onTriggered: {
                    calib_timer.start();
                    console.log("waiting_timer")
                }
        }

        Connections {
            target: app
            onClosing: {
                    console.log("closiiing...");
                    if (viewStack.depth > 1) {
                        viewStack.pop();
                        close.accepted = false;
                    } else {
                        close.accepted = true;
                    }
                }
            }
        }

    property bool pulseDisplayVisible_parent: false

    VideoPage {
        id: videoPage
    }


    Item {
        id: page1
        visible:false

        Rectangle{
            width: app.width
            height: app.height

                Text{
                    id: calib_text
                    text: "Calibration start"
                    anchors.centerIn: parent
                    font.pixelSize: 30
                    font.family: "Bookman"
                }

                Button{
                    text: "Ready?"
                    anchors.top: calib_text.bottom
                    anchors.topMargin: 5
                    anchors.horizontalCenter: calib_text.horizontalCenter
                    font.family: "Bookman"
                    onClicked:{
                        viewStack.replace(page2)

                    }
                }
        }
    }

    Item {
        id: page2
        visible: false

        Rectangle{
            width: app.width
            height: app.height

            Text{
                id: normal
                text: "Keep your eyes in a normal open state for 5 seconds"
                anchors.centerIn: parent
                font.pixelSize: 30
                font.family: "Bookman"
            }

            Button{
                text: "Ready?"
                anchors.top: normal.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: normal.horizontalCenter
                font.family: "Bookman"
                onClicked: {
                    controller.currentState = "normal";
                    viewStack.state = "normal";
                    first_timer.start();

                }
            }
        }
    }


    Item {
        id: page3
        visible:false

        Rectangle{
            width: app.width
            height: app.height

            Text{
                id: fully_opened
                text: "Fully open the eyes and maintain this state for 5 seconds"
                anchors.centerIn: parent
                font.pixelSize: 30
                font.family: "Bookman"
            }

            Button{
                text: "Ready?"
                anchors.top: fully_opened.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: fully_opened.horizontalCenter
                font.family: "Bookman"
                onClicked:{
                    controller.currentState = "fully_opened";
                    viewStack.state = "fully_opened";
                    calib_timer.start();

                }

            }
        }

    }

    Item {
        id: page4
        visible: false

        Rectangle{
            width: app.width
            height: app.height

            Text{
                id: fully_closed
                text: "Close the eyes and hold this position for 5 seconds"
                anchors.centerIn: parent
                font.pixelSize: 30
                font.family: "Bookman"
            }

            Button{
                text: "Ready?"
                anchors.top: fully_closed.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: fully_closed.horizontalCenter
                font.family: "Bookman"
                onClicked: {
                    controller.currentState = "fully_closed";
                    viewStack.state = "fully_closed";
                    calib_timer.start();
                }
            }
        }
    }



    Item {
        id: page5
        visible: false

        Rectangle{
            width: app.width
            height: app.height

            Text{
                id: end
                text: "This is the end of the calibration, thank you for your help"
                anchors.centerIn: parent
                font.pixelSize: 30
                font.family: "Bookman"
            }

            Button{
                text: "Start"
                anchors.top: end.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: end.horizontalCenter
                font.family: "Bookman"
                onClicked: {
                    controller.currentState = "videopage";
                    viewStack.state = "videopage";
                    controller.endOfTheCalibration();
                    controller.startEARPlotSignal();
                }
            }
        }
    }


    PulseDisplay {
        id: pulse_display
        anchors.right: parent.right
        visible: pulseDisplayVisible_parent
    }

    footer: FooterToolbar{
        id: footer
        visible: toolbar.landmarkWorking
   }

}





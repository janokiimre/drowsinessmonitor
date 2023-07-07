import QtQuick 2.10
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.7


Page {
    id: page
    bottomPadding: 0
    topPadding: 0
    height: parent.height
    width: parent.width
    anchors.bottom: parent.bottom
    visible:false

    Rectangle{

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#023B74" }
            GradientStop { position: 0.25; color: "#0E539F" }
            GradientStop { position: 0.5; color: "#3C6BB5" }
            GradientStop { position: 0.75; color: "#6B8CC8" }
            GradientStop { position: 1.0; color: "#A7BCDE" }
        }

        anchors.fill:parent
        anchors.bottom: parent.bottom

        AnimatedImage {
            id: gif2
            source: "qrc:/Images/loading-gif.gif"
            height: 40
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
        }
    }

    Item {
        id: previewItem
        anchors.fill: parent
        /*
        Label {
            text: "No Basler camera detected."
            anchors.centerIn: parent
            visible: !camera.isOpen
        }
        */


        VideoOutput {
            id: viewFinder
            anchors.fill: parent
            visible: true
            source: controller //?
            fillMode: VideoOutput.Stretch
            autoOrientation: true

        }
    }
}


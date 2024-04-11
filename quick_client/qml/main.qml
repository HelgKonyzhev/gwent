import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window
{
    width: 640
    height: 480
    visible: true
//    visibility: Window.FullScreen
    title: qsTr("Hello World")

    Loader
    {
        id: stateLoader
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: statusLabel.top

        function onDoorstepEntered()
        {
            source = "qrc:/qml/doorstep.qml"
        }
    }

    Label
    {
        id: statusLabel
        text: "Disconnected"
        color: "black"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Connections
    {
        target: socket

        function onConnected()
        {
            statusLabel.text = "Connected"
        }

        function onDisconnected()
        {
            statusLabel.text = "Disconnected"
            stateLoader.source = ""
        }
    }

    Connections
    {
        target: player.doorstepState

        function onEntered()
        {
            stateLoader.source = "qrc:/qml/doorstep.qml"
            statusLabel.text = "Doorstep"
        }
    }

    Connections
    {
        target: player.lobbyState

        function onEntered()
        {
            stateLoader.source = "qrc:/qml/lobby.qml"
            statusLabel.text = "Lobby"
        }
    }
}

import QtQuick
import QtQuick.Controls
import common.events

Item {
    Column
    {
        anchors.centerIn: parent
        width: 200

        TextInput
        {
            id: loginInput
            width: parent.width
            color: "black"
            text: "Username"
        }

        TextInput
        {
            id: passwordInput
            width: parent.width
            color: "black"
            text: "Password"
        }


        Row
        {
            Button
            {
                id: loginButton
                text: "Login"
                onClicked:
                {
                    player.login(loginInput.text, passwordInput.text)
                }
            }

            Button
            {
                id: registerButton
                text: "Register"
                onClicked:
                {
                    player.registerPlayer(loginInput.text, passwordInput.text)
                }
            }

            spacing: 10
        }
    }

    Connections
    {
        target: player

        function onLoggedIn(loginEvent)
        {
            statusLabel.text = "Logged in"
        }

        function onLoginFailed(loginFailedEvent)
        {
            statusLabel.text = "Login failed: " + loginFailedEvent.reason
        }

        function onRegistered(registeredEvent)
        {
            statusLabel.text = "Registered"
        }

        function onRegistrationFailed(registrationFailedEvent)
        {
            statusLabel.text = "Registration failed: " + registrationFailedEvent.reason
        }
    }
}

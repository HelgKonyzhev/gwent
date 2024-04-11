import QtQuick
import QtQuick.Controls

Item {
    Item {
        anchors.left: parent.left
        height: parent.height
        width: parent.width / 3

        Label {
            id: decksLabel
            anchors.top: parent.top
            color: "black"
            text: "Decks:"
        }

        ListModel {
            id: decksListModel
        }

        ListView {
            id: decksListView
            anchors.top: decksLabel.bottom
            anchors.bottom: addDeckRow.top
            width: parent.width

            model: decksListModel

            delegate: Component {
                id: nameDelegate
                Text {
                    required property string name
                    text: name
                }
            }

            leftMargin: 5
            rightMargin: 5
            topMargin: 5
            bottomMargin: 5
        }

        Row {
            id: addDeckRow
            anchors.bottom: parent.bottom

            Button {
                id: addDeckButton
                text: "Add deck"

                onClicked: {
                    decksListModel.append({
                                              "name": deckNameInput.text
                                          })
                }
            }

            TextInput {
                id: deckNameInput
                text: "New deck"
            }

            spacing: 10
        }
    }
}

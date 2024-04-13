import QtQuick
import QtQuick.Controls

Item {
    signal save
    signal cancel

    Item {
        id: deckCardsItem
        anchors.top: parent.top
        anchors.bottom: controlsRow.top
        anchors.left: parent.left
        width: parent.width / 3

        Label {
            id: deckCardsLabel
            anchors.top: parent.top
            anchors.left: parent.left
            text: "Deck cards:"
            color: "black"
        }

        ListModel {
            id: deckCardsModel

            function reload() {
                clear()

                for (var i = 0; i < player.editingDeck.all.length; ++i) {
                    var card = player.editingDeck.all[i]

                    if (card.tags.includes("leader"))
                        continue

                    append({
                               "name": card.name
                           })
                }
            }

            Component.onCompleted: {
                player.editingDeck.cardAdded.connect(reload)
                player.editingDeck.cardErased.connect(reload)

                reload()
            }
        }

        ListView {
            id: decksListView
            anchors.top: deckCardsLabel.bottom
            anchors.bottom: parent.bottom
            width: parent.width

            model: deckCardsModel

            delegate: Component {
                ItemDelegate {
                    width: decksListView.width - decksListView.leftMargin
                           - decksListView.rightMargin
                    text: name
                    palette.text: "black"
                    highlighted: ListView.isCurrentItem
                    onClicked: decksListView.currentIndex = index

                    background: Rectangle {
                        color: highlighted ? "black" : "white"
                    }
                }
            }

            leftMargin: 5
            rightMargin: 5
            topMargin: 5
            bottomMargin: 5
        }
    }

    Item {
        id: deckInfoItem
        anchors.top: parent.top
        anchors.bottom: controlsRow.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 3

        Row {
            id: deckNameRow
            anchors.top: parent.top
            anchors.left: parent.left

            Label {
                text: "Deck name:"
                color: "black"
            }

            TextInput {
                id: deckNameInput
                color: "black"
                text: player.editingDeck.name.length == 0 ? "New deck" : player.editingDeck.name
            }
            spacing: 5
        }

        Row {
            id: deckFractionRow
            anchors.top: deckNameRow.bottom
            anchors.left: parent.left

            Label {
                text: "Fraction:"
                color: "black"
            }

            TextInput {
                id: deckFractionInput
                color: "black"
                text: player.editingDeck.fraction
            }

            spacing: 5
        }

        Row {
            id: deckLeaderNameRow
            anchors.top: deckFractionRow.bottom
            anchors.left: parent.left

            Label {
                text: "Leader:"
                color: "black"
            }

            TextInput {
                id: deckLeaderInput
                text: player.editingDeck.leader.name
                color: "black"
            }

            spacing: 5
        }

        Image {
            id: deckLeaderImage
            anchors.top: deckLeaderNameRow.bottom
            anchors.bottom: parent.bottom
            source: "qrc:/" + player.editingDeck.leader.image
            autoTransform: true
            fillMode: Image.PreserveAspectFit
        }
    }

    Item {
        id: availableCardsItem
        anchors.top: parent.top
        anchors.bottom: controlsRow.top
        anchors.right: parent.right
        width: parent.width / 3

        Label {
            id: availableCardsLabel
            anchors.top: parent.top
            anchors.left: parent.left
            text: "Available cards:"
            color: "black"
        }

        ListModel {
            id: availableCardsModel

            Component.onCompleted: {
                for (var i = 0; i < cardsData.all.length; ++i)
                    append({
                               "name": cardsData.all[i].name
                           })
            }
        }

        ListView {
            id: availableListView
            anchors.top: availableCardsLabel.bottom
            anchors.bottom: parent.bottom
            width: parent.width

            model: availableCardsModel

            delegate: Component {
                ItemDelegate {
                    width: availableListView.width - availableListView.leftMargin
                           - availableListView.rightMargin
                    text: name
                    palette.text: "black"
                    highlighted: ListView.isCurrentItem
                    onClicked: {
                        availableListView.currentIndex = index
                    }

                    background: Rectangle {
                        color: highlighted ? "black" : "white"
                    }

                    onDoubleClicked: {
                        player.editingDeck.addCard(name)
                    }
                }
            }

            leftMargin: 5
            rightMargin: 5
            topMargin: 5
            bottomMargin: 5
        }
    }

    Row {
        id: controlsRow
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Button {
            id: saveButton
            text: "Save"
            onClicked: {
                player.editingDeck.name = deckNameInput.text
                player.editingDeck.fraction = deckFractionInput.text
                save()
            }
        }

        Button {
            id: cancelButton
            text: "Cancel"
            onClicked: cancel()
        }

        spacing: 10
    }
}

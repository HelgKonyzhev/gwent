import QtQuick
import QtQuick.Controls

Item {
    signal addDeck

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
                ItemDelegate {
                    width: decksListView.width - decksListView.leftMargin
                           - decksListView.rightMargin
                    text: name
                    palette.text: "black"
                    highlighted: ListView.isCurrentItem

                    onClicked: {
                        decksListView.currentIndex = index
                    }

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

        Row {
            id: addDeckRow
            anchors.bottom: parent.bottom

            Button {
                id: addDeckButton
                text: "Add deck"

                onClicked: {
                    addDeck()
                }
            }

            spacing: 10
        }
    }

    function reload() {//        console.log(player.data.name)
        //        var deckNames = player.data.deckNames
        //        for (var i = 0; i < deckNames.length; ++i)
        //            decksListModel.append({
        //                                      "name": deckNames[i]
        //                                  })
    }

    Component.onCompleted: {
        reload()
    }
}

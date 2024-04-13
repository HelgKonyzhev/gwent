import QtQuick
import QtQuick.Controls

Item {
    Loader {
        id: decksViewerLoader
        source: "qrc:/qml/decks_viewer.qml"
        anchors.fill: parent

        onLoaded: {
            item.addDeck.connect(onAddDeck)
        }
    }

    Loader {
        id: deckEditorLoader
        visible: false
        anchors.fill: parent

        onLoaded: {
            item.save.connect(onSave)
            item.cancel.connect(onCancel)
        }
    }

    function onAddDeck() {
        player.addDeck()

        decksViewerLoader.visible = false
        deckEditorLoader.visible = true

        decksViewerLoader.source = ""
        deckEditorLoader.source = "qrc:/qml/deck_editor.qml"
    }

    function onSave() {
        decksViewerLoader.visible = true
        deckEditorLoader.visible = false

        decksViewerLoader.source = "qrc:/qml/decks_viewer.qml"
        deckEditorLoader.source = ""

        player.saveDeck()
    }

    function onCancel() {
        decksViewerLoader.visible = true
        deckEditorLoader.visible = false

        decksViewerLoader.source = "qrc:/qml/decks_viewer.qml"
        deckEditorLoader.source = ""

        player.cancelEdition()
    }
}

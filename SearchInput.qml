import QtQuick 1.1



Item {
    id: searchItem; objectName: "searchInput"
    width: parent.width; height: 30
    signal queryChanged(string query)

    BorderImage {
        source: "assets/imgs/lineedit.sci"
        anchors.fill: parent
    }

    TextInput {
        id: input
        horizontalAlignment: TextEdit.AlignLeft
        font.pixelSize: 16; font.bold: true; color: "black"

        anchors {
            left: parent.left; right: parent.right; leftMargin: 18; rightMargin: 18
            verticalCenter: parent.verticalCenter
        }

        onTextChanged: timer.start()
        onAccepted: {
            searchItem.queryChanged(text)
            timer.stop();
        }
    }

    Timer {
        id: timer
        interval: 1000; running: false; repeat: true
        onTriggered: {
            searchItem.queryChanged(input.text)
            timer.stop();
        }
    }
}

import QtQuick 1.1



Item {
    id: search

    property alias url: urlText.text

    width: parent.height; height: parent.height

    BorderImage {
        id: bg; rotation: 180
        x: 8; width: parent.width - 16; height: 30;
        anchors.verticalCenter: parent.verticalCenter
        border { left: 10; top: 10; right: 10; bottom: 10 }
    }

    TextInput {
        id: urlText
        horizontalAlignment: TextEdit.AlignLeft
        font.pixelSize: 14;

        anchors {
            left: parent.left; right: parent.right; leftMargin: 18; rightMargin: 18
            verticalCenter: parent.verticalCenter
        }
    }
}

import QtQuick 1.1

Item {
    id: delegate
    focus: true
    width: delegate.ListView.view.width
    height: 130

    Image {
        id: thumb
        height: parent.height - 10; y: 5
        width: 140
        smooth: true
        source: image

        anchors {
            left: parent.left
            leftMargin: 5
            topMargin: 5
        }
    }

    Text {
        id: text
        text: title
        color: delegate.ListView.isCurrentItem ? "white" : "gray"
        wrapMode: Text.WordWrap
        font { family: "Helvetica"; pixelSize: 16; bold: true }

        anchors {
            top: thumb.top
            left: thumb.right
            leftMargin: 5
        }
    }

    Text {
        id: desc
        text: description
        height: parent.height
        color: delegate.ListView.isCurrentItem ? "white" : "gray"
        font { family: "Helvetica"; pixelSize: 12 }
        wrapMode: Text.WordWrap
        anchors {
            top: text.bottom
            left: thumb.right
            bottom: thumb.bottom
            right: parent.right
            margins: 5, 5, 5, 5
        }
    }

    MouseArea {
        anchors.fill: delegate
        onClicked: {
            delegate.ListView.view.currentIndex = index
            window.currentUrl = url
            window.currentThumb = image
        }
    }

    Keys.onReleased: {
        window.currentThumb = thumb

        if (event.key == Qt.Key_Return) {
            window.currentUrl = url
        }
    }

    Keys.onPressed: {
        window.ensureHudVisible();
    }
}

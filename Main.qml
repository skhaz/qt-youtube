import QtQuick 1.1
import OmniMedia 1.0



Rectangle {
    id: window
    width: 1024; height: 768; color: "black"

    property Media currentMedia

    function ensureHudVisible() {
        panel.state = "HUDON"
        // timer.restart()
    }

    Timer {
        id: timer
        interval: 3000; running: true; repeat: true
        // onTriggered: panel.state = "HUDOFF"
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: window
        onHoveredChanged: ensureHudVisible()
    }

    Player {
        source: window.currentMedia
        id: videoPlayer

        // VideoControl {
        //    x: (parent.width / 2) - (width / 2)
        //    y: (parent.height - 20) - height
        //    visible: false
        //}
    }

    Rectangle {
        id: panel
        focus: true
        width: 400; height: window.height; y: searchInput.height + searchInput.y
        color: "black"; opacity: .8

        Component {
            id: highlight
            Rectangle {
                color: "white"; opacity: .2
                y: list.currentItem.y
                Behavior on y {
                    SpringAnimation {
                        spring: 3
                        damping: 0.2
                    }
                }
            }
        }

        ListView {
            id: list
            focus: true
            anchors.fill: parent
            model: youtubeModel
            delegate: Delegate {}
            highlight: highlight
        }

        Rectangle {
            width: parent.width; height: 20

            gradient: Gradient {
                GradientStop { position: 0.0; color: "black" }
                GradientStop { position: 1.0; color: "transparent" }
            }
        }

        Rectangle {
            width: parent.width; height: 20
            anchors.bottom: list.bottom

            gradient: Gradient {
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 1.0; color: "black" }
            }
        }

        ScrollBar { scrollArea: list; height: list.height; width: 8; anchors.right: list.right }

        states : [
            State {
                name: "HUDON"
                PropertyChanges {
                    target: panel
                    x: 0
                }
            },

            State {
                name: "HUDOFF"
                PropertyChanges {
                    target: panel
                    x: -panel.width
                }
            }
        ]

        transitions: [
            Transition {
                from: "HUDON"
                to: "HUDOFF"
                PropertyAnimation {
                    duration: 1000; properties: "x"
                    easing.type: Easing.OutCubic
                }
            },

            Transition {
                from: "HUDOFF"
                to: "HUDON"
                PropertyAnimation {
                    duration: 1000; properties: "x"
                    easing.type: Easing.OutCubic
                }
            }
        ]
    }

    SearchInput {
        id: searchInput
        width: panel.width
    }
}

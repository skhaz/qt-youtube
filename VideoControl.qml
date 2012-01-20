import QtQuick 1.1



Rectangle {
    id: control
    width: 350; height: 70
    radius: 6.0
    smooth: true

    MouseArea {
        anchors.fill: parent
        drag.target: control
    }

    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#CC444444"
        }

        GradientStop {
            position: .3
            color: "#CC000000"
        }

        GradientStop {
            position: 1
            color: "#EE000000"
        }
    }
}

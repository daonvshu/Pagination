import QtQuick 2.15

Rectangle {
    id: btnbackground

    state: "normal"

    height: 24
    radius: 4

    property alias text: displayText.text
    property var padding: 0

    property var colorHover: "#3F3F3F"

    property var selectable: false
    property var selected: false

    signal btnSelected

    width: displayText.implicitWidth + padding

    onSelectedChanged: {
        state = selected ? "selected" : "normal"
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: btnbackground
                color: "transparent"
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: btnbackground
                color: colorHover
            }
        },
        State {
            name: "press"
            PropertyChanges {
                target: btnbackground
                color: Qt.lighter(colorHover, 1.2)
            }
        },
        State {
            name: "selected"
            PropertyChanges {
                target: btnbackground
                color: Qt.lighter(colorHover, 1.2)
            }
        }

    ]

    transitions: [
        Transition {
            from: "hover"
            to: "press"

            ColorAnimation {
                target: btnbackground
                properties: "color"
                duration: 120
            }
        }
    ]

    Text {
        id: displayText
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        color: "white"
        font.pixelSize: 13
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            if (!selected) {
                parent.state = "hover"
            }
        }

        onExited: {
            if (parent.state != "press" && !selected) {
                parent.state = "normal"
            }
        }

        onPressed: {
            if (!selectable) {
                parent.state = "press"
                btnSelected()
            }
        }

        onReleased: {
            if (!selectable) {
                if (containsMouse) {
                    parent.state = "hover"
                } else {
                    parent.state = "normal"
                }
            } else if (containsMouse) {
                selected = true
                btnSelected()
            }
        }
    }
}

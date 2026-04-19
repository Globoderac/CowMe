import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: welcomePage
    signal startClicked()
    signal hallOfFameClicked()
    required property QtObject theme
    background: Rectangle { color: theme.background }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 0
        width: Math.min(parent.width - 60, 500)

        // ??? Hero Card ???
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 250
            radius: 16
            color: theme.surface
            border.color: theme.border
            border.width: 1

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 0

                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    width: 96; height: 96; radius: 48
                    color: theme.background
                    border.color: theme.border

                    Image {
                        id: logoImg
                        anchors.centerIn: parent
                        width: 76; height: 76
                        source: "qrc:/assets/wlogo.png"
                        fillMode: Image.PreserveAspectFit
                        visible: status === Image.Ready
                    }
                    Label {
                        anchors.centerIn: parent
                        text: "CowMe"
                        font { pixelSize: 20; weight: Font.Bold }
                        color: theme.accent
                        visible: !logoImg.visible
                    }
                }

                Label {
                    text: "CowMe"
                    font { pixelSize: 32; weight: Font.Bold; letterSpacing: -0.5 }
                    color: theme.textPrimary
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 14
                }

                Label {
                    text: "QUIZ TRAINER"
                    font { pixelSize: 11; weight: Font.DemiBold; letterSpacing: 5 }
                    color: theme.accent
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 4
                }

                Label {
                    text: "Master modern C++ through interactive quizzes"
                    font.pixelSize: 13
                    color: theme.textSecondary
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 12
                }
            }
        }

        // ??? Feature Cards ???
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 16
            spacing: 12

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 130
                radius: 12
                color: theme.surface
                border.color: theme.border
                clip: true

                Image {
                    anchors.fill: parent
                    anchors.margins: 1
                    source: "qrc:/assets/cpp_mascot.jpg"
                    fillMode: Image.PreserveAspectCrop
                    opacity: 0.12
                }

                ColumnLayout {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.margins: 16
                    spacing: 2

                    Label {
                        text: "C++ Quizzes"
                        font { pixelSize: 15; weight: Font.Bold }
                        color: theme.textPrimary
                    }
                    Label {
                        text: "From basics to C++23"
                        font.pixelSize: 12
                        color: theme.textSecondary
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 130
                radius: 12
                color: theme.surface
                border.color: theme.border
                clip: true

                Image {
                    anchors.fill: parent
                    anchors.margins: 1
                    source: "qrc:/assets/vessel.jpg"
                    fillMode: Image.PreserveAspectCrop
                    opacity: 0.12
                }

                ColumnLayout {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.margins: 16
                    spacing: 2

                    Label {
                        text: "Industry Ready"
                        font { pixelSize: 15; weight: Font.Bold }
                        color: theme.textPrimary
                    }
                    Label {
                        text: "Real-world scenarios"
                        font.pixelSize: 12
                        color: theme.textSecondary
                    }
                }
            }
        }

        // ??? Start Button (full-width, rounded) ???
        Button {
            id: startBtn
            Layout.fillWidth: true
            Layout.topMargin: 24
            Layout.preferredHeight: 50

            contentItem: Label {
                text: "Begin Training"
                font { pixelSize: 15; weight: Font.DemiBold; letterSpacing: 0.3 }
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 12
                color: startBtn.pressed ? theme.accentDim
                     : startBtn.hovered ? theme.accentHover
                     : theme.accent
                Behavior on color { ColorAnimation { duration: 150 } }
            }

            onClicked: welcomePage.startClicked()
        }

        Button {
            id: hofBtn
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.preferredHeight: 44

            contentItem: Label {
                text: "Hall of Fame"
                font { pixelSize: 14; weight: Font.DemiBold }
                color: theme.accent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 12
                color: hofBtn.pressed ? theme.surfaceLight
                     : hofBtn.hovered ? Qt.rgba(0, 0.39, 0.64, 0.06)
                     : "transparent"
                border.color: theme.accent
                border.width: 1
                Behavior on color { ColorAnimation { duration: 150 } }
            }

            onClicked: welcomePage.hallOfFameClicked()
        }

        Label {
            text: "v0.1.0"
            font.pixelSize: 11
            color: theme.textMuted
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 20
        }
    }
}

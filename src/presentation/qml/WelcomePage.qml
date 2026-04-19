import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Page {
    id: welcomePage
    signal startClicked()
    signal hallOfFameClicked()
    required property QtObject theme
    background: Rectangle { color: theme.background }

    // Watermark background
    Image {
        anchors.centerIn: parent
        width: Math.min(parent.width, parent.height) * 0.6
        height: width
        source: "qrc:/assets/wartsila_text.png"
        fillMode: Image.PreserveAspectFit
        opacity: 0.04
    }

    // Floating decorative shapes
    Repeater {
        model: [
            { x: 0.08, y: 0.12, sz: 80,  r: 40, clr: "#0063A3", op: 0.06, dur: 6000 },
            { x: 0.85, y: 0.08, sz: 60,  r: 12, clr: "#0063A3", op: 0.05, dur: 8000 },
            { x: 0.92, y: 0.75, sz: 100, r: 50, clr: "#0063A3", op: 0.04, dur: 7000 },
            { x: 0.05, y: 0.80, sz: 50,  r: 8,  clr: "#0063A3", op: 0.06, dur: 5000 },
            { x: 0.70, y: 0.40, sz: 40,  r: 20, clr: "#0063A3", op: 0.05, dur: 9000 },
            { x: 0.30, y: 0.90, sz: 70,  r: 35, clr: "#0063A3", op: 0.04, dur: 7500 }
        ]
        delegate: Rectangle {
            required property var modelData
            x: welcomePage.width * modelData.x
            y: welcomePage.height * modelData.y
            width: modelData.sz; height: modelData.sz
            radius: modelData.r
            color: modelData.clr
            opacity: modelData.op
            SequentialAnimation on y {
                loops: Animation.Infinite
                NumberAnimation { to: welcomePage.height * modelData.y - 15; duration: modelData.dur; easing.type: Easing.InOutSine }
                NumberAnimation { to: welcomePage.height * modelData.y + 15; duration: modelData.dur; easing.type: Easing.InOutSine }
            }
        }
    }

    // Accent gradient line at top
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 3
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.3; color: theme.accent }
            GradientStop { position: 0.7; color: theme.accent }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }

    Flickable {
        anchors.fill: parent
        contentWidth: width
        contentHeight: mainCol.implicitHeight + 60
        boundsBehavior: Flickable.StopAtBounds

    ColumnLayout {
        id: mainCol
        anchors.horizontalCenter: parent.horizontalCenter
        y: 40
        width: Math.min(parent.width - 60, 620)
        spacing: 0

        // Navigation bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Image {
                source: "qrc:/assets/wlogo.png"
                Layout.preferredWidth: 32; Layout.preferredHeight: 32
                fillMode: Image.PreserveAspectFit
            }
            Label {
                text: "WaskMe"
                font { pixelSize: 16; weight: Font.Bold }
                color: theme.textPrimary
            }
            Item { Layout.fillWidth: true }
            Label {
                text: "v0.1.0"
                font.pixelSize: 11
                color: theme.textMuted
            }
        }

        // Divider
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 12
            height: 1
            color: theme.border
        }

        // Hero section
        ColumnLayout {
            Layout.fillWidth: true
            Layout.topMargin: 50
            spacing: 0

            Label {
                text: "QUIZ TRAINER"
                font { pixelSize: 12; weight: Font.DemiBold; letterSpacing: 6 }
                color: theme.accent
                Layout.alignment: Qt.AlignHCenter
                opacity: 0
                NumberAnimation on opacity { from: 0; to: 1; duration: 500 }
            }

            Label {
                text: "Master Modern C++"
                font { pixelSize: 44; weight: Font.Bold; letterSpacing: -1.5 }
                color: theme.textPrimary
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                opacity: 0
                NumberAnimation on opacity { from: 0; to: 1; duration: 600; easing.type: Easing.OutCubic }
            }

            Label {
                text: "Interactive quizzes to sharpen your C++20/23 skills.\nFrom fundamentals to real-world industry scenarios."
                font.pixelSize: 15
                lineHeight: 1.6
                lineHeightMode: Text.ProportionalHeight
                color: theme.textSecondary
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                Layout.maximumWidth: 460
                wrapMode: Text.Wrap
                opacity: 0
                NumberAnimation on opacity { from: 0; to: 1; duration: 700 }
            }

            // CTA buttons
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 32
                spacing: 14

                Button {
                    id: startBtn
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 52
                    contentItem: Label {
                        text: "Begin Training"
                        font { pixelSize: 15; weight: Font.DemiBold; letterSpacing: 0.3 }
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 26
                        color: startBtn.pressed ? theme.accentDim
                             : startBtn.hovered ? theme.accentHover
                             : theme.accent
                        Behavior on color { ColorAnimation { duration: 150 } }
                    }
                    onClicked: welcomePage.startClicked()
                }

                Button {
                    id: hofBtn
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 52
                    contentItem: Label {
                        text: "Hall of Fame"
                        font { pixelSize: 14; weight: Font.DemiBold }
                        color: theme.accent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 26
                        color: hofBtn.pressed ? theme.surfaceLight
                             : hofBtn.hovered ? Qt.rgba(0, 0.39, 0.64, 0.06)
                             : "transparent"
                        border.color: theme.accent
                        border.width: 1.5
                        Behavior on color { ColorAnimation { duration: 150 } }
                    }
                    onClicked: welcomePage.hallOfFameClicked()
                }
            }
        }

        // Feature cards - 3 column grid
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 60
            spacing: 16

            Repeater {
                model: [
                    { icon: "qrc:/assets/cpp_mascot.jpg", title: "C++ Quizzes", desc: "From basics to C++23 standards", tag: "50+ Questions" },
                    { icon: "qrc:/assets/vessel.jpg", title: "Industry Ready", desc: "Real-world marine & energy scenarios", tag: "Practical" },
                    { icon: "qrc:/assets/wlogo.png", title: "Track Progress", desc: "Compete on the leaderboard", tag: "Hall of Fame" }
                ]
                delegate: Rectangle {
                    required property var modelData
                    required property int index
                    Layout.fillWidth: true
                    Layout.preferredHeight: 200
                    radius: 14
                    color: theme.surface
                    border.color: cardMa.containsMouse ? theme.accent : theme.border
                    border.width: cardMa.containsMouse ? 1.5 : 1
                    clip: true
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    MouseArea {
                        id: cardMa
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.NoButton
                    }

                    Image {
                        anchors.fill: parent
                        anchors.margins: 1
                        source: modelData.icon
                        fillMode: Image.PreserveAspectCrop
                        opacity: 0.08
                    }

                    ColumnLayout {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.margins: 18
                        spacing: 4

                        Rectangle {
                            width: tagLabel.implicitWidth + 14; height: 22; radius: 11
                            color: Qt.rgba(0, 0.39, 0.64, 0.1)
                            Label {
                                id: tagLabel
                                anchors.centerIn: parent
                                text: modelData.tag
                                font { pixelSize: 10; weight: Font.DemiBold }
                                color: theme.accent
                            }
                        }

                        Label {
                            text: modelData.title
                            font { pixelSize: 16; weight: Font.Bold }
                            color: theme.textPrimary
                            Layout.topMargin: 4
                        }
                        Label {
                            text: modelData.desc
                            font.pixelSize: 12
                            color: theme.textSecondary
                            wrapMode: Text.Wrap
                            Layout.fillWidth: true
                        }
                    }

                    // Entry animation
                    opacity: 0; y: 20
                    NumberAnimation on opacity { from: 0; to: 1; duration: 500; easing.type: Easing.OutCubic }
                    NumberAnimation on y { from: 20; to: 0; duration: 500; easing.type: Easing.OutCubic }
                    Component.onCompleted: {
                        opacity = 0
                    }
                }
            }
        }

        // Footer
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 50
            height: 1
            color: theme.border
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 12
            Layout.bottomMargin: 20

            Label {
                text: "WaskMe Quiz Trainer"
                font.pixelSize: 11
                color: theme.textMuted
            }
            Item { Layout.fillWidth: true }
            Label {
                text: "Wartsila"
                font { pixelSize: 11; weight: Font.DemiBold }
                color: theme.textMuted
            }
        }
    }
    }
}

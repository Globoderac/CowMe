import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: hofPage
    signal backClicked()
    required property QtObject theme
    background: Rectangle { color: theme.background }

    Component.onCompleted: hallOfFameVM.loadAll()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 0

        // Header
        RowLayout {
            spacing: 12

            Button {
                id: backBtn
                Layout.preferredWidth: 36; Layout.preferredHeight: 36
                contentItem: Label {
                    text: "←"; font.pixelSize: 18; color: theme.textSecondary
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    radius: 8
                    color: backBtn.hovered ? theme.surfaceLight : "transparent"
                    Behavior on color { ColorAnimation { duration: 120 } }
                }
                onClicked: hofPage.backClicked()
            }

            Label {
                text: "🏆  Hall of Fame"
                font { pixelSize: 22; weight: Font.Bold }
                color: theme.textPrimary
                Layout.fillWidth: true
            }
        }

        Label {
            Layout.topMargin: 8
            text: "Top scores ranked by correct answers, then fastest time"
            font.pixelSize: 12
            color: theme.textSecondary
        }

        // Table header
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 20
            height: 40; radius: 8
            color: theme.surfaceLight

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16; anchors.rightMargin: 16
                spacing: 8

                Label {
                    text: "#"
                    font { pixelSize: 12; weight: Font.DemiBold }
                    color: theme.textSecondary
                    Layout.preferredWidth: 32
                }
                Label {
                    text: "Player"
                    font { pixelSize: 12; weight: Font.DemiBold }
                    color: theme.textSecondary
                    Layout.preferredWidth: 140
                }
                Label {
                    text: "Quiz"
                    font { pixelSize: 12; weight: Font.DemiBold }
                    color: theme.textSecondary
                    Layout.fillWidth: true
                }
                Label {
                    text: "Score"
                    font { pixelSize: 12; weight: Font.DemiBold }
                    color: theme.textSecondary
                    Layout.preferredWidth: 80
                    horizontalAlignment: Text.AlignHCenter
                }
                Label {
                    text: "Time"
                    font { pixelSize: 12; weight: Font.DemiBold }
                    color: theme.textSecondary
                    Layout.preferredWidth: 70
                    horizontalAlignment: Text.AlignHCenter
                }
                Label {
                    text: "Date"
                    font { pixelSize: 12; weight: Font.DemiBold }
                    color: theme.textSecondary
                    Layout.preferredWidth: 110
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        // Empty state
        Label {
            visible: hallOfFameVM.entries.length === 0
            Layout.fillWidth: true
            Layout.topMargin: 60
            text: "No scores yet — complete a quiz to be the first!"
            font.pixelSize: 14
            color: theme.textMuted
            horizontalAlignment: Text.AlignHCenter
        }

        // Entries
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 4
            contentWidth: width
            contentHeight: entriesCol.implicitHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            ColumnLayout {
                id: entriesCol
                width: parent.width
                spacing: 0

                Repeater {
                    model: hallOfFameVM.entries
                    delegate: Rectangle {
                        required property var modelData
                        required property int index
                        Layout.fillWidth: true
                        height: 44; radius: 4
                        color: index % 2 === 0 ? theme.surface : theme.background

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16; anchors.rightMargin: 16
                            spacing: 8

                            // Rank with medal for top 3
                            Label {
                                text: {
                                    if (index === 0) return "🥇"
                                    if (index === 1) return "🥈"
                                    if (index === 2) return "🥉"
                                    return (index + 1).toString()
                                }
                                font { pixelSize: index < 3 ? 16 : 12; weight: Font.Bold }
                                color: theme.textPrimary
                                Layout.preferredWidth: 32
                            }

                            Label {
                                text: modelData.playerName || ""
                                font { pixelSize: 13; weight: index < 3 ? Font.Bold : Font.Normal }
                                color: theme.textPrimary
                                elide: Text.ElideRight
                                Layout.preferredWidth: 140
                            }

                            Label {
                                text: modelData.quizName || ""
                                font.pixelSize: 12
                                color: theme.textSecondary
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Label {
                                text: modelData.score + " / " + modelData.totalQuestions
                                font { pixelSize: 13; weight: Font.DemiBold }
                                color: theme.accent
                                Layout.preferredWidth: 80
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Label {
                                text: {
                                    var s = modelData.elapsedSeconds
                                    var mins = Math.floor(s / 60)
                                    var secs = s % 60
                                    return mins + ":" + (secs < 10 ? "0" : "") + secs
                                }
                                font { pixelSize: 12; family: "Consolas" }
                                color: theme.textSecondary
                                Layout.preferredWidth: 70
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Label {
                                text: modelData.timestamp || ""
                                font.pixelSize: 11
                                color: theme.textMuted
                                Layout.preferredWidth: 110
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                }
            }
        }
    }
}

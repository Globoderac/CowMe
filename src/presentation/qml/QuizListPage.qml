import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: quizListPage
    signal quizSelected(int quizId)
    signal backClicked()
    signal createQuizClicked()
    signal editQuizClicked(int quizId, string name, string description)
    required property QtObject theme
    background: Rectangle { color: theme.background }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 0

        // Top bar
        RowLayout {
            spacing: 12

            Button {
                id: backBtn
                Layout.preferredWidth: 36
                Layout.preferredHeight: 36
                contentItem: Label {
                    text: "←"
                    font.pixelSize: 18
                    color: theme.textSecondary
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    radius: 8
                    color: backBtn.hovered ? theme.surfaceLight : "transparent"
                    border.color: backBtn.hovered ? theme.border : "transparent"
                    Behavior on color { ColorAnimation { duration: 120 } }
                }
                onClicked: quizListPage.backClicked()
            }

            ColumnLayout {
                spacing: 2
                Label {
                    text: "Select Quiz"
                    font { pixelSize: 22; weight: Font.Bold }
                    color: theme.textPrimary
                }
                Label {
                    text: quizListVM.quizCount + " quizzes available"
                    font.pixelSize: 12
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                id: createBtn
                Layout.preferredHeight: 34
                contentItem: Label {
                    text: "+ Create"
                    font { pixelSize: 13; weight: Font.DemiBold }
                    color: theme.accent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    radius: 8; implicitWidth: 90
                    color: createBtn.hovered ? Qt.rgba(0, 0.71, 0.85, 0.12) : "transparent"
                    border.color: theme.accent
                    Behavior on color { ColorAnimation { duration: 120 } }
                }
                onClicked: quizListPage.createQuizClicked()
            }

            // Status indicator
            Rectangle {
                visible: quizListVM.loading
                width: 8; height: 8; radius: 4
                color: theme.accent
                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation { to: 0.3; duration: 600 }
                    NumberAnimation { to: 1.0; duration: 600 }
                }
            }
        }

        // Error banner
        Rectangle {
            visible: quizListVM.errorMessage.length > 0
            Layout.fillWidth: true
            Layout.topMargin: 16
            height: 44; radius: 8
            color: theme.errorBg
            border.color: Qt.rgba(1, 0.32, 0.32, 0.25)

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                Label {
                    text: "!"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    color: theme.error
                }
                Label {
                    text: quizListVM.errorMessage
                    font.pixelSize: 13
                    color: theme.error
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }

        // Separator
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 20
            Layout.bottomMargin: 16
            height: 1
            color: theme.border
        }

        // Quiz cards
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: quizListVM.quizCount
            clip: true
            spacing: 12
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                id: card
                required property int index
                width: ListView.view ? ListView.view.width : 0
                height: 90
                radius: 10

                property bool isHovered: cardArea.containsMouse
                    || editBtn.hovered || deleteBtn.hovered

                color: isHovered ? theme.surfaceLight : theme.surface
                border.color: isHovered ? theme.accent : theme.border
                border.width: 1

                Behavior on color { ColorAnimation { duration: 150 } }
                Behavior on border.color { ColorAnimation { duration: 150 } }

                MouseArea {
                    id: cardArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.LeftButton
                    onClicked: {
                        var qid = quizListVM.quizIdAt(card.index)
                        quizListPage.quizSelected(qid)
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    // Icon badge
                    Rectangle {
                        width: 48; height: 48; radius: 10
                        color: Qt.rgba(0, 0.39, 0.64, 0.08)
                        border.color: Qt.rgba(0, 0.39, 0.64, 0.15)

                        Label {
                            anchors.centerIn: parent
                            text: {
                                var name = quizListVM.quizNames[card.index] || "Q"
                                return name.charAt(0).toUpperCase()
                            }
                            font { pixelSize: 20; weight: Font.Bold }
                            color: theme.accent
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Label {
                            text: quizListVM.quizNames[card.index] || ""
                            font { pixelSize: 16; weight: Font.DemiBold }
                            color: theme.textPrimary
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        Label {
                            text: quizListVM.quizDescriptionAt(card.index)
                            font.pixelSize: 12
                            color: theme.textSecondary
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                    // Question count badge
                    Rectangle {
                        width: countLabel.implicitWidth + 20
                        height: 28; radius: 14
                        color: theme.surfaceLight
                        border.color: theme.border

                        Label {
                            id: countLabel
                            anchors.centerIn: parent
                            text: quizListVM.quizQuestionCountAt(card.index) + " Q"
                            font { pixelSize: 11; weight: Font.Medium }
                            color: theme.accent
                        }
                    }

                    // Edit button
                    Button {
                        id: editBtn
                        Layout.preferredWidth: 32; Layout.preferredHeight: 32
                        opacity: card.isHovered ? 1.0 : 0.0
                        Behavior on opacity { NumberAnimation { duration: 150 } }
                        z: 1
                        contentItem: Label {
                            text: "✎"
                            font.pixelSize: 14
                            color: editBtn.hovered ? theme.accent : theme.textSecondary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            Behavior on color { ColorAnimation { duration: 120 } }
                        }
                        background: Rectangle {
                            radius: 6
                            color: editBtn.hovered ? Qt.rgba(0, 0.71, 0.85, 0.12) : "transparent"
                            Behavior on color { ColorAnimation { duration: 120 } }
                        }
                        onClicked: {
                            var qid = quizListVM.quizIdAt(card.index)
                            var qname = quizListVM.quizNames[card.index] || ""
                            var qdesc = quizListVM.quizDescriptionAt(card.index)
                            quizListPage.editQuizClicked(qid, qname, qdesc)
                        }
                    }

                    // Delete button
                    Button {
                        id: deleteBtn
                        Layout.preferredWidth: 32; Layout.preferredHeight: 32
                        opacity: card.isHovered ? 1.0 : 0.0
                        Behavior on opacity { NumberAnimation { duration: 150 } }
                        z: 1
                        contentItem: Label {
                            text: "✕"
                            font.pixelSize: 13
                            color: deleteBtn.hovered ? theme.error : theme.textMuted
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            Behavior on color { ColorAnimation { duration: 120 } }
                        }
                        background: Rectangle {
                            radius: 6
                            color: deleteBtn.hovered ? theme.errorBg : "transparent"
                            Behavior on color { ColorAnimation { duration: 120 } }
                        }
                        onClicked: quizListVM.deleteQuiz(quizListVM.quizIdAt(card.index))
                    }
                }
            }
        }
    }

    Component.onCompleted: quizListVM.loadQuizzes()
    StackView.onActivating: quizListVM.loadQuizzes()
}

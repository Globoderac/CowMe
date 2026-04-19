import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 960
    height: 680
    minimumWidth: 800
    minimumHeight: 560
    visible: true
    visibility: Window.FullScreen
    title: "WaskMe - Wartsila"
    color: appTheme.background

    Shortcut {
        sequence: "F11"
        onActivated: root.visibility = Window.FullScreen
    }
    Shortcut {
        sequence: "Escape"
        onActivated: root.visibility = Window.Windowed
    }
    Shortcut {
        sequence: "F12"
        onActivated: hallOfFameVM.devMode = !hallOfFameVM.devMode
    }

    QtObject {
        id: appTheme
        readonly property color background:    "#f5f6f8"
        readonly property color surface:       "#ffffff"
        readonly property color surfaceLight:  "#eef0f4"
        readonly property color border:        "#d8dce5"
        readonly property color accent:        "#0063a3"
        readonly property color accentHover:   "#004f85"
        readonly property color accentDim:     "#00538c"
        readonly property color textPrimary:   "#1a1d23"
        readonly property color textSecondary: "#5a6170"
        readonly property color textMuted:     "#9099a8"
        readonly property color success:       "#1a8c3f"
        readonly property color successBg:     "#e6f7ec"
        readonly property color error:         "#c62828"
        readonly property color errorBg:       "#fdecea"
        readonly property color codeBg:        "#f0f2f5"
        readonly property color progressTrack: "#dfe2e8"
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: welcomePage

        pushEnter: Transition {
            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 250; easing.type: Easing.OutCubic }
            PropertyAnimation { property: "x"; from: 60; to: 0; duration: 250; easing.type: Easing.OutCubic }
        }
        pushExit: Transition {
            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200 }
        }
        popEnter: Transition {
            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 250; easing.type: Easing.OutCubic }
            PropertyAnimation { property: "x"; from: -60; to: 0; duration: 250; easing.type: Easing.OutCubic }
        }
        popExit: Transition {
            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200 }
        }
    }

    Component {
        id: welcomePage
        WelcomePage {
            theme: appTheme
            onStartClicked: stackView.push(quizListPage)
            onHallOfFameClicked: stackView.push(hallOfFamePage)
        }
    }

    Component {
        id: quizListPage
        QuizListPage {
            theme: appTheme
            onQuizSelected: function(quizId) {
                quizStartDialog.selectedQuizId = quizId
                quizStartDialog.maxQuestions = quizSessionVM.availableQuestionCount(quizId)
                quizStartDialog.questionSlider.value = quizStartDialog.maxQuestions
                quizStartDialog.timeSlider.value = 0
                quizStartDialog.open()
            }
            onBackClicked: stackView.pop()
            onCreateQuizClicked: {
                createQuizVM.reset()
                stackView.push(createQuizPage)
            }
            onEditQuizClicked: function(quizId, name, description) {
                createQuizVM.loadForEdit(quizId, name, description)
                stackView.push(createQuizPage)
            }
        }
    }

    // ─── Quiz Start Dialog (question count selector) ───
    Popup {
        id: quizStartDialog
        anchors.centerIn: parent
        width: 380
        height: dialogCol.implicitHeight + 48
        modal: true
        dim: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        property int selectedQuizId: -1
        property int maxQuestions: 0
        property alias questionSlider: qSlider
        property alias timeSlider: tSlider

        background: Rectangle {
            radius: 12
            color: appTheme.surface
            border.color: appTheme.border
            border.width: 1
        }

        Overlay.modal: Rectangle {
            color: Qt.rgba(0, 0, 0, 0.35)
        }

        ColumnLayout {
            id: dialogCol
            anchors.fill: parent
            anchors.margins: 24
            spacing: 20

            Label {
                text: "Configure Quiz"
                font { pixelSize: 18; weight: Font.Bold }
                color: appTheme.textPrimary
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: appTheme.border
            }

            ColumnLayout {
                spacing: 8
                Layout.fillWidth: true

                Label {
                    text: "Your name"
                    font { pixelSize: 13; weight: Font.Medium }
                    color: appTheme.textSecondary
                }

                TextField {
                    id: playerNameField
                    Layout.fillWidth: true
                    placeholderText: "Enter your name..."
                    font.pixelSize: 13
                    color: appTheme.textPrimary
                    background: Rectangle {
                        radius: 8
                        color: appTheme.surfaceLight
                        border.color: playerNameField.activeFocus ? appTheme.accent : appTheme.border
                        border.width: playerNameField.activeFocus ? 2 : 1
                        Behavior on border.color { ColorAnimation { duration: 120 } }
                    }
                }

                Label {
                    text: "Name is used for the Hall of Fame leaderboard"
                    font.pixelSize: 11
                    color: appTheme.textMuted
                }
            }

            ColumnLayout {
                spacing: 8
                Layout.fillWidth: true

                Label {
                    text: "Number of questions"
                    font { pixelSize: 13; weight: Font.Medium }
                    color: appTheme.textSecondary
                }

                RowLayout {
                    spacing: 12
                    Layout.fillWidth: true

                    Slider {
                        id: qSlider
                        Layout.fillWidth: true
                        from: 1
                        to: quizStartDialog.maxQuestions > 0 ? quizStartDialog.maxQuestions : 1
                        stepSize: 1
                        value: quizStartDialog.maxQuestions

                        background: Rectangle {
                            x: qSlider.leftPadding
                            y: qSlider.topPadding + qSlider.availableHeight / 2 - height / 2
                            implicitWidth: 200; implicitHeight: 4
                            width: qSlider.availableWidth; height: implicitHeight
                            radius: 2; color: appTheme.progressTrack

                            Rectangle {
                                width: qSlider.visualPosition * parent.width
                                height: parent.height; radius: 2
                                color: appTheme.accent
                            }
                        }

                        handle: Rectangle {
                            x: qSlider.leftPadding + qSlider.visualPosition * (qSlider.availableWidth - width)
                            y: qSlider.topPadding + qSlider.availableHeight / 2 - height / 2
                            width: 20; height: 20; radius: 10
                            color: qSlider.pressed ? appTheme.accentHover : appTheme.accent
                            border.color: appTheme.accentDim
                        }
                    }

                    Rectangle {
                        width: 44; height: 28; radius: 6
                        color: appTheme.surfaceLight
                        border.color: appTheme.border
                        Label {
                            anchors.centerIn: parent
                            text: Math.round(qSlider.value)
                            font { pixelSize: 14; weight: Font.Bold }
                            color: appTheme.accent
                        }
                    }
                }

                Label {
                    text: {
                        var n = Math.round(qSlider.value)
                        var max = quizStartDialog.maxQuestions
                        if (n >= max)
                            return "All questions (randomized order)"
                        var pct = max > 0 ? n / max : 0
                        if (pct >= 0.9)
                            return "Absolute masochist detected. We salute your suffering."
                        if (pct >= 0.75)
                            return "You enjoy pain, don't you? Bold choice."
                        if (pct >= 0.5)
                            return "Ambitious! Hope your coffee is strong."
                        if (pct >= 0.3)
                            return "Balanced mix: ~30% Easy, ~35% Medium, ~25% Hard, ~10% Expert"
                        return "Just warming up? That's okay, we don't judge... much."
                    }
                    font.pixelSize: 11
                    color: {
                        var n = Math.round(qSlider.value)
                        var max = quizStartDialog.maxQuestions
                        var pct = max > 0 ? n / max : 0
                        if (pct >= 0.9) return appTheme.error
                        if (pct >= 0.75) return "#e6a817"
                        return appTheme.textMuted
                    }
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }
            }

            // ─── Time Limit Section ───
            ColumnLayout {
                spacing: 8
                Layout.fillWidth: true

                Label {
                    text: "Time limit"
                    font { pixelSize: 13; weight: Font.Medium }
                    color: appTheme.textSecondary
                }

                RowLayout {
                    spacing: 12
                    Layout.fillWidth: true

                    Slider {
                        id: tSlider
                        Layout.fillWidth: true
                        from: 0
                        to: 60
                        stepSize: 1
                        value: 0

                        background: Rectangle {
                            x: tSlider.leftPadding
                            y: tSlider.topPadding + tSlider.availableHeight / 2 - height / 2
                            implicitWidth: 200; implicitHeight: 4
                            width: tSlider.availableWidth; height: implicitHeight
                            radius: 2; color: appTheme.progressTrack

                            Rectangle {
                                width: tSlider.visualPosition * parent.width
                                height: parent.height; radius: 2
                                color: appTheme.accent
                            }
                        }

                        handle: Rectangle {
                            x: tSlider.leftPadding + tSlider.visualPosition * (tSlider.availableWidth - width)
                            y: tSlider.topPadding + tSlider.availableHeight / 2 - height / 2
                            width: 20; height: 20; radius: 10
                            color: tSlider.pressed ? appTheme.accentHover : appTheme.accent
                            border.color: appTheme.accentDim
                        }
                    }

                    Rectangle {
                        width: 56; height: 28; radius: 6
                        color: appTheme.surfaceLight
                        border.color: appTheme.border
                        Label {
                            anchors.centerIn: parent
                            text: {
                                var m = Math.round(tSlider.value)
                                return m === 0 ? "Off" : m + " min"
                            }
                            font { pixelSize: 13; weight: Font.Bold }
                            color: Math.round(tSlider.value) === 0 ? appTheme.textMuted : appTheme.accent
                        }
                    }
                }

                Label {
                    text: Math.round(tSlider.value) === 0
                        ? "No time limit — take as long as you need"
                        : "Quiz will auto-submit when time runs out"
                    font.pixelSize: 11
                    color: appTheme.textMuted
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Item { Layout.fillWidth: true }

                Button {
                    id: cancelStartBtn
                    Layout.preferredHeight: 38
                    contentItem: Label {
                        text: "Cancel"
                        font { pixelSize: 13 }
                        color: appTheme.textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 8; implicitWidth: 80
                        color: cancelStartBtn.hovered ? appTheme.surfaceLight : "transparent"
                        border.color: appTheme.border
                        Behavior on color { ColorAnimation { duration: 120 } }
                    }
                    onClicked: quizStartDialog.close()
                }

                Button {
                    id: goBtn
                    Layout.preferredHeight: 38
                    contentItem: Label {
                        text: "Start Quiz"
                        font { pixelSize: 13; weight: Font.DemiBold }
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 8; implicitWidth: 120
                        color: goBtn.hovered ? appTheme.accentHover : appTheme.accent
                        Behavior on color { ColorAnimation { duration: 120 } }
                    }
                    onClicked: {
                        var count = Math.round(qSlider.value)
                        var timeSecs = Math.round(tSlider.value) * 60
                        quizSessionVM.playerName = playerNameField.text.trim()
                        quizStartDialog.close()
                        stackView.push(quizSessionPage, {
                            quizId: quizStartDialog.selectedQuizId,
                            questionCount: count >= quizStartDialog.maxQuestions ? -1 : count,
                            timeLimitSecs: timeSecs
                        })
                    }
                }
            }
        }
    }

    Component {
        id: quizSessionPage
        QuizSessionPage {
            theme: appTheme
            onBackClicked: stackView.pop()
        }
    }

    Component {
        id: createQuizPage
        CreateQuizPage {
            theme: appTheme
            onBackClicked: {
                quizListVM.loadQuizzes()
                stackView.pop()
            }
        }
    }

    Component {
        id: hallOfFamePage
        HallOfFamePage {
            theme: appTheme
            onBackClicked: stackView.pop()
        }
    }
}

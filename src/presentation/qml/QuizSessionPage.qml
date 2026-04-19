import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: quizSessionPage
    signal backClicked()
    required property QtObject theme
    property int quizId: -1
    property int questionCount: -1
    property int timeLimitSecs: 0
    background: Rectangle { color: theme.background }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 0

        // ─── Top Header Bar ───
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
                onClicked: quizSessionPage.backClicked()
            }

            Label {
                text: quizSessionVM.quizName
                font { pixelSize: 18; weight: Font.Bold }
                color: theme.textPrimary
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            // Timer pill
            Rectangle {
                visible: quizSessionVM.timerEnabled
                width: timerRow.implicitWidth + 20; height: 30; radius: 15
                color: quizSessionVM.remainingSeconds <= 60 && quizSessionVM.remainingSeconds > 0
                    ? theme.errorBg : theme.surface
                border.color: quizSessionVM.remainingSeconds <= 60 && quizSessionVM.remainingSeconds > 0
                    ? Qt.rgba(0.78, 0.16, 0.16, 0.3) : theme.border

                RowLayout {
                    id: timerRow
                    anchors.centerIn: parent
                    spacing: 4
                    Label {
                        text: "\u23F1"
                        font.pixelSize: 14
                    }
                    Label {
                        text: {
                            var s = quizSessionVM.remainingSeconds
                            var mins = Math.floor(s / 60)
                            var secs = s % 60
                            return mins + ":" + (secs < 10 ? "0" : "") + secs
                        }
                        font { pixelSize: 13; weight: Font.Bold; family: "Consolas" }
                        color: quizSessionVM.remainingSeconds <= 60 && quizSessionVM.remainingSeconds > 0
                            ? theme.error : theme.textPrimary
                    }
                }
            }

            // Score pill
            Rectangle {
                width: scoreRow.implicitWidth + 20; height: 30; radius: 15
                color: theme.surface; border.color: theme.border

                RowLayout {
                    id: scoreRow
                    anchors.centerIn: parent
                    spacing: 6
                    Label {
                        text: "Score:"
                        font.pixelSize: 12
                        font.weight: Font.Medium
                        color: theme.accent
                    }
                    Label {
                        text: quizSessionVM.score + " / " + quizSessionVM.totalQuestions
                        font { pixelSize: 13; weight: Font.Medium }
                        color: theme.textPrimary
                    }
                }
            }
        }

        // ─── Progress Bar ───
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 16
            height: 4; radius: 2
            color: theme.progressTrack

            Rectangle {
                width: quizSessionVM.totalQuestions > 0
                    ? parent.width * (quizSessionVM.currentIndex + 1) / quizSessionVM.totalQuestions
                    : 0
                height: parent.height; radius: 2
                color: theme.accent

                Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.OutCubic } }
            }
        }

        Label {
            Layout.topMargin: 6
            text: "Question " + (quizSessionVM.currentIndex + 1) + " of " + quizSessionVM.totalQuestions
            font { pixelSize: 11; weight: Font.Medium }
            color: theme.textMuted
        }

        // ─── Finished State ───
        Flickable {
            visible: quizSessionVM.isFinished
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 20
            contentWidth: width
            contentHeight: finishedCol.implicitHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.VerticalFlick

        ColumnLayout {
            id: finishedCol
            width: parent.width
            spacing: 0

            // Animated badge
            Rectangle {
                id: doneBadge
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                width: 90; height: 90; radius: 45
                color: Qt.rgba(0.1, 0.55, 0.25, 0.08)
                border.color: Qt.rgba(0.1, 0.55, 0.25, 0.25)
                scale: 0; opacity: 0

                Label {
                    anchors.centerIn: parent
                    text: {
                        var pct = quizSessionVM.totalQuestions > 0
                            ? Math.round(100 * quizSessionVM.score / quizSessionVM.totalQuestions) : 0
                        return pct + "%"
                    }
                    font { pixelSize: 24; weight: Font.Bold }
                    color: theme.success
                }

                SequentialAnimation on scale {
                    running: quizSessionVM.isFinished
                    PauseAnimation { duration: 100 }
                    NumberAnimation { to: 1.1; duration: 300; easing.type: Easing.OutBack }
                    NumberAnimation { to: 1.0; duration: 150; easing.type: Easing.InOutQuad }
                }
                NumberAnimation on opacity {
                    running: quizSessionVM.isFinished
                    from: 0; to: 1; duration: 300
                }
            }

            Label {
                id: completeLabel
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                text: quizSessionVM.timeExpired ? "Time's Up!" : "Quiz Complete"
                font { pixelSize: 26; weight: Font.Bold }
                color: theme.textPrimary
                opacity: 0
                NumberAnimation on opacity {
                    running: quizSessionVM.isFinished
                    from: 0; to: 1; duration: 400; easing.type: Easing.InOutQuad
                }
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 8
                text: quizSessionVM.score + " / " + quizSessionVM.totalQuestions + " correct"
                font.pixelSize: 15
                color: theme.textSecondary
                opacity: 0
                NumberAnimation on opacity {
                    running: quizSessionVM.isFinished
                    from: 0; to: 1; duration: 400
                }
            }

            // Result progress bar
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                width: 260; height: 8; radius: 4
                color: theme.progressTrack

                Rectangle {
                    id: resultBar
                    width: 0
                    height: parent.height; radius: 4
                    color: {
                        var pct = quizSessionVM.totalQuestions > 0
                            ? quizSessionVM.score / quizSessionVM.totalQuestions : 0
                        return pct >= 0.7 ? theme.success : (pct >= 0.4 ? "#e6a817" : theme.error)
                    }
                    NumberAnimation on width {
                        running: quizSessionVM.isFinished
                        from: 0
                        to: quizSessionVM.totalQuestions > 0
                            ? 260 * quizSessionVM.score / quizSessionVM.totalQuestions : 0
                        duration: 600; easing.type: Easing.OutCubic
                    }
                }
            }

            // Results table header
            Label {
                Layout.topMargin: 32
                Layout.leftMargin: 4
                text: "Results"
                font { pixelSize: 16; weight: Font.DemiBold }
                color: theme.textPrimary
            }

            // Table header row
            Rectangle {
                Layout.fillWidth: true
                Layout.topMargin: 10
                height: 36; radius: 6
                color: theme.surfaceLight

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12; anchors.rightMargin: 12
                    spacing: 8

                    Label {
                        text: "#"
                        font { pixelSize: 12; weight: Font.DemiBold }
                        color: theme.textSecondary
                        Layout.preferredWidth: 28
                    }
                    Label {
                        text: "Question"
                        font { pixelSize: 12; weight: Font.DemiBold }
                        color: theme.textSecondary
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Your Answer"
                        font { pixelSize: 12; weight: Font.DemiBold }
                        color: theme.textSecondary
                        Layout.preferredWidth: 120
                    }
                    Label {
                        text: ""
                        Layout.preferredWidth: 28
                    }
                }
            }

            // Table rows
            Repeater {
                model: quizSessionVM.answerResults
                delegate: Rectangle {
                    required property var modelData
                    required property int index
                    Layout.fillWidth: true
                    height: 42; radius: 4
                    color: index % 2 === 0 ? theme.surface : theme.background

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12; anchors.rightMargin: 12
                        spacing: 8

                        Label {
                            text: (index + 1).toString()
                            font.pixelSize: 12
                            color: theme.textMuted
                            Layout.preferredWidth: 28
                        }
                        Label {
                            text: modelData.question || ""
                            font.pixelSize: 12
                            color: theme.textPrimary
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                        Label {
                            text: modelData.userAnswer || ""
                            font.pixelSize: 12
                            color: modelData.correct ? theme.success : theme.error
                            elide: Text.ElideRight
                            Layout.preferredWidth: 120
                        }
                        Rectangle {
                            width: 22; height: 22; radius: 11
                            color: modelData.correct
                                ? Qt.rgba(0.1, 0.55, 0.25, 0.1)
                                : Qt.rgba(0.78, 0.16, 0.16, 0.1)
                            Layout.preferredWidth: 28

                            Label {
                                anchors.centerIn: parent
                                text: modelData.correct ? "✓" : "✗"
                                font { pixelSize: 13; weight: Font.Bold }
                                color: modelData.correct ? theme.success : theme.error
                            }
                        }
                    }
                }
            }

            // Incorrect answers detail
            Label {
                visible: {
                    var results = quizSessionVM.answerResults
                    for (var i = 0; i < results.length; i++)
                        if (!results[i].correct) return true
                    return false
                }
                Layout.topMargin: 24
                Layout.leftMargin: 4
                text: "Correct Answers for Missed Questions"
                font { pixelSize: 14; weight: Font.DemiBold }
                color: theme.textSecondary
            }

            Repeater {
                model: quizSessionVM.answerResults
                delegate: Rectangle {
                    required property var modelData
                    required property int index
                    visible: !modelData.correct
                    Layout.fillWidth: true
                    Layout.topMargin: visible ? 6 : 0
                    height: visible ? wrongCol.implicitHeight + 16 : 0
                    radius: 8
                    color: theme.errorBg
                    border.color: Qt.rgba(0.78, 0.16, 0.16, 0.12)

                    ColumnLayout {
                        id: wrongCol
                        anchors.left: parent.left; anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: 10
                        spacing: 4

                        Label {
                            text: modelData.question || ""
                            font { pixelSize: 12; weight: Font.DemiBold }
                            color: theme.textPrimary
                            wrapMode: Text.Wrap
                            Layout.fillWidth: true
                        }
                        Label {
                            text: "Your answer: " + (modelData.userAnswer || "")
                            font.pixelSize: 11
                            color: theme.error
                        }
                        Label {
                            text: "Correct: " + (modelData.correctAnswer || "")
                            font.pixelSize: 11
                            color: theme.success
                        }
                    }
                }
            }

            // Back button
            Button {
                id: doneBtn
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 32
                Layout.preferredWidth: 200; Layout.preferredHeight: 44
                contentItem: Label {
                    text: "Back to Quizzes"
                    font { pixelSize: 14; weight: Font.DemiBold }
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    radius: 8
                    color: doneBtn.hovered ? theme.accentHover : theme.accent
                    Behavior on color { ColorAnimation { duration: 150 } }
                }
                onClicked: quizSessionPage.backClicked()
            }

            Item { height: 30 }
        }
        }

        // ─── Active Question State ───
        Flickable {
            visible: !quizSessionVM.isFinished
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 20
            contentWidth: width
            contentHeight: activeCol.implicitHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.VerticalFlick

        ColumnLayout {
            id: activeCol
            width: parent.width
            spacing: 0

            // Question card
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: questionCol.implicitHeight + 32
                radius: 10
                color: theme.surface
                border.color: theme.border

                ColumnLayout {
                    id: questionCol
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 16
                    spacing: 12

                    Label {
                        text: quizSessionVM.questionTitle
                        font { pixelSize: 16; weight: Font.DemiBold }
                        color: theme.textPrimary
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }

                    // Code block
                    Rectangle {
                        visible: quizSessionVM.codeSnippet.length > 0
                        Layout.fillWidth: true
                        Layout.preferredHeight: codeText.implicitHeight + 24
                        radius: 8
                        color: theme.codeBg
                        border.color: theme.border

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 0

                            // Line number gutter
                            Label {
                                Layout.alignment: Qt.AlignTop
                                text: {
                                    var lines = quizSessionVM.codeSnippet.split("\n")
                                    var nums = []
                                    for (var i = 0; i < lines.length; i++)
                                        nums.push(i + 1)
                                    return nums.join("\n")
                                }
                                font { family: "Consolas"; pixelSize: 13 }
                                color: theme.textMuted
                                lineHeight: 1.5
                                Layout.preferredWidth: 28
                            }

                            Rectangle {
                                Layout.fillHeight: true
                                Layout.leftMargin: 8; Layout.rightMargin: 12
                                width: 1; color: theme.border
                            }

                            Label {
                                id: codeText
                                text: quizSessionVM.codeSnippet
                                font { family: "Consolas"; pixelSize: 13 }
                                color: theme.accent
                                wrapMode: Text.Wrap
                                lineHeight: 1.5
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignTop
                            }
                        }
                    }
                }
            }

            // ─── Answer Buttons ───
            ColumnLayout {
                visible: quizSessionVM.answerOptions.length > 0
                Layout.fillWidth: true
                Layout.topMargin: 16
                spacing: 8

                Repeater {
                    model: quizSessionVM.answerOptions
                    delegate: Button {
                        id: optionBtn
                        required property string modelData
                        required property int index
                        Layout.fillWidth: true
                        Layout.preferredHeight: 46

                        enabled: !quizSessionVM.answerLocked

                        contentItem: RowLayout {
                            spacing: 12
                            anchors.leftMargin: 16; anchors.rightMargin: 16
                            anchors.left: parent.left; anchors.right: parent.right

                            // Option letter badge
                            Rectangle {
                                width: 26; height: 26; radius: 13
                                color: "transparent"
                                border.color: optionBtn.hovered && !quizSessionVM.answerLocked
                                    ? theme.accent : theme.textMuted
                                border.width: 1
                                Behavior on border.color { ColorAnimation { duration: 120 } }

                                Label {
                                    anchors.centerIn: parent
                                    text: String.fromCharCode(65 + optionBtn.index)
                                    font { pixelSize: 11; weight: Font.Bold }
                                    color: optionBtn.hovered && !quizSessionVM.answerLocked
                                        ? theme.accent : theme.textSecondary
                                }
                            }

                            Label {
                                text: optionBtn.modelData
                                font.pixelSize: 14
                                color: theme.textPrimary
                                wrapMode: Text.Wrap
                                Layout.fillWidth: true
                            }
                        }

                        background: Rectangle {
                            radius: 8
                            color: {
                                if (!quizSessionVM.answerLocked)
                                    return optionBtn.hovered ? theme.surfaceLight : theme.surface
                                return theme.surface
                            }
                            border.color: {
                                if (!quizSessionVM.answerLocked)
                                    return optionBtn.hovered ? theme.accent : theme.border
                                return theme.border
                            }
                            border.width: 1
                            Behavior on color { ColorAnimation { duration: 120 } }
                            Behavior on border.color { ColorAnimation { duration: 120 } }
                        }

                        onClicked: quizSessionVM.submitAnswer(optionBtn.modelData)
                    }
                }
            }

            // ─── Text Input (when no options) ───
            RowLayout {
                visible: quizSessionVM.answerOptions.length === 0 && !quizSessionVM.answerLocked
                Layout.fillWidth: true
                Layout.topMargin: 16
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    height: 44; radius: 8
                    color: theme.surface
                    border.color: answerField.activeFocus ? theme.accent : theme.border
                    Behavior on border.color { ColorAnimation { duration: 120 } }

                    TextInput {
                        id: answerField
                        anchors.fill: parent
                        anchors.margins: 12
                        font.pixelSize: 14
                        color: theme.textPrimary
                        clip: true
                        verticalAlignment: TextInput.AlignVCenter
                        onAccepted: {
                            if (text.length > 0) {
                                quizSessionVM.submitAnswer(text)
                                text = ""
                            }
                        }

                        Text {
                            visible: !answerField.text
                            text: "Type your answer…"
                            font: answerField.font
                            color: theme.textMuted
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                Button {
                    id: submitBtn
                    Layout.preferredWidth: 90; Layout.preferredHeight: 44
                    contentItem: Label {
                        text: "Submit"
                        font { pixelSize: 13; weight: Font.DemiBold }
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 8
                        color: submitBtn.hovered ? theme.accentHover : theme.accent
                        Behavior on color { ColorAnimation { duration: 150 } }
                    }
                    onClicked: {
                        if (answerField.text.length > 0) {
                            quizSessionVM.submitAnswer(answerField.text)
                            answerField.text = ""
                        }
                    }
                }
            }

            // ─── Feedback Banner ───
            Rectangle {
                visible: quizSessionVM.lastFeedback.length > 0
                Layout.fillWidth: true
                Layout.topMargin: 16
                Layout.preferredHeight: feedbackContent.implicitHeight + 24
                radius: 10
                color: quizSessionVM.lastAnswerCorrect ? theme.successBg : theme.errorBg
                border.color: quizSessionVM.lastAnswerCorrect
                    ? Qt.rgba(0, 0.9, 0.46, 0.2) : Qt.rgba(1, 0.32, 0.32, 0.2)

                RowLayout {
                    id: feedbackContent
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 12
                    spacing: 10

                    Rectangle {
                        width: 32; height: 32; radius: 16
                        color: quizSessionVM.lastAnswerCorrect
                            ? Qt.rgba(0, 0.9, 0.46, 0.15) : Qt.rgba(1, 0.32, 0.32, 0.15)
                        Layout.alignment: Qt.AlignTop

                        Label {
                            anchors.centerIn: parent
                            text: quizSessionVM.lastAnswerCorrect ? "✓" : "✗"
                            font { pixelSize: 16; weight: Font.Bold }
                            color: quizSessionVM.lastAnswerCorrect ? theme.success : theme.error
                        }
                    }

                    Label {
                        text: quizSessionVM.lastFeedback
                        font.pixelSize: 13
                        color: quizSessionVM.lastAnswerCorrect ? theme.success : "#ff8a80"
                        wrapMode: Text.Wrap
                        lineHeight: 1.4
                        Layout.fillWidth: true
                    }
                }
            }

            Item { height: 16 }

            // ─── Next / Continue Button ───
            Button {
                id: nextBtn
                visible: quizSessionVM.answerLocked
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: 160; Layout.preferredHeight: 42
                contentItem: RowLayout {
                    anchors.centerIn: parent
                    spacing: 6
                    Label {
                        text: (quizSessionVM.currentIndex + 1 >= quizSessionVM.totalQuestions)
                            ? "Finish" : "Next"
                        font { pixelSize: 14; weight: Font.DemiBold }
                        color: "#ffffff"
                    }
                    Label {
                        text: "→"; font.pixelSize: 14; color: "#ffffff"
                    }
                }
                background: Rectangle {
                    radius: 8
                    color: nextBtn.hovered ? theme.accentHover : theme.accent
                    Behavior on color { ColorAnimation { duration: 150 } }
                }
                onClicked: quizSessionVM.nextQuestion()
            }

            Item { height: 20 }
        }
        }
    }

    Component.onCompleted: {
        if (quizId > 0) {
            if (timeLimitSecs > 0)
                quizSessionVM.startQuizWithCountAndTime(quizId, questionCount > 0 ? questionCount : -1, timeLimitSecs)
            else if (questionCount > 0)
                quizSessionVM.startQuizWithCount(quizId, questionCount)
            else
                quizSessionVM.startQuiz(quizId)
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: createQuizPage
    signal backClicked()
    required property QtObject theme
    background: Rectangle { color: theme.background }

    // State for the inline "add question" form
    property bool addingQuestion: false
    property int editingQuestionIndex: -1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 0

        // ── Top bar ──
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
                onClicked: createQuizPage.backClicked()
            }

            Label {
                text: createQuizVM.editMode ? "Edit Quiz" : "Create Quiz"
                font { pixelSize: 22; weight: Font.Bold }
                color: theme.textPrimary
            }

            Item { Layout.fillWidth: true }

            // Save button
            Button {
                id: saveBtn
                enabled: createQuizVM.canSave && !createQuizVM.saving
                Layout.preferredHeight: 36
                contentItem: Label {
                    text: createQuizVM.saving ? "Saving…" : (createQuizVM.editMode ? "Update Quiz" : "Save Quiz")
                    font { pixelSize: 13; weight: Font.DemiBold }
                    color: saveBtn.enabled ? "#ffffff" : theme.textMuted
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    radius: 8
                    color: saveBtn.enabled
                        ? (saveBtn.hovered ? theme.accentHover : theme.accent)
                        : theme.surfaceLight
                    border.color: saveBtn.enabled ? "transparent" : theme.border
                    Behavior on color { ColorAnimation { duration: 120 } }
                    implicitWidth: 110
                }
                onClicked: createQuizVM.save()
            }
        }

        // ── Error banner ──
        Rectangle {
            visible: createQuizVM.errorMessage.length > 0
            Layout.fillWidth: true
            Layout.topMargin: 12
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
                    text: createQuizVM.errorMessage
                    font.pixelSize: 13; color: theme.error
                    elide: Text.ElideRight; Layout.fillWidth: true
                }
            }
        }

        // ── Separator ──
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 20; Layout.bottomMargin: 16
            height: 1; color: theme.border
        }

        // ── Scrollable content ──
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: contentCol.implicitHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            ColumnLayout {
                id: contentCol
                width: parent.width
                spacing: 20

                // ── Quiz info fields ──
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: quizInfoCol.implicitHeight + 32
                    radius: 10; color: theme.surface
                    border.color: theme.border

                    ColumnLayout {
                        id: quizInfoCol
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Label {
                            text: "Quiz Details"
                            font { pixelSize: 14; weight: Font.DemiBold }
                            color: theme.accent
                        }

                        // Name
                        ColumnLayout {
                            spacing: 4; Layout.fillWidth: true
                            Label { text: "Name"; font.pixelSize: 12; color: theme.textSecondary }
                            TextField {
                                id: nameField
                                Layout.fillWidth: true
                                placeholderText: "e.g. Modern C++ Features"
                                text: createQuizVM.quizName
                                onTextChanged: createQuizVM.quizName = text
                                color: theme.textPrimary
                                placeholderTextColor: theme.textMuted
                                font.pixelSize: 14
                                background: Rectangle {
                                    radius: 6; color: theme.surfaceLight
                                    border.color: nameField.activeFocus ? theme.accent : theme.border
                                    Behavior on border.color { ColorAnimation { duration: 150 } }
                                }
                            }
                        }

                        // Description
                        ColumnLayout {
                            spacing: 4; Layout.fillWidth: true
                            Label { text: "Description"; font.pixelSize: 12; color: theme.textSecondary }
                            TextField {
                                id: descField
                                Layout.fillWidth: true
                                placeholderText: "Optional description"
                                text: createQuizVM.quizDescription
                                onTextChanged: createQuizVM.quizDescription = text
                                color: theme.textPrimary
                                placeholderTextColor: theme.textMuted
                                font.pixelSize: 14
                                background: Rectangle {
                                    radius: 6; color: theme.surfaceLight
                                    border.color: descField.activeFocus ? theme.accent : theme.border
                                    Behavior on border.color { ColorAnimation { duration: 150 } }
                                }
                            }
                        }
                    }
                }

                // ── Questions header ──
                RowLayout {
                    spacing: 8

                    Label {
                        text: "Questions"
                        font { pixelSize: 14; weight: Font.DemiBold }
                        color: theme.accent
                    }

                    Rectangle {
                        width: qCountLabel.implicitWidth + 14
                        height: 22; radius: 11
                        color: theme.surfaceLight; border.color: theme.border
                        Label {
                            id: qCountLabel
                            anchors.centerIn: parent
                            text: createQuizVM.questionCount
                            font { pixelSize: 11; weight: Font.Medium }
                            color: theme.accent
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Button {
                        id: addQBtn
                        Layout.preferredHeight: 32
                        contentItem: Label {
                            text: "+ Add Question"
                            font { pixelSize: 12; weight: Font.DemiBold }
                            color: theme.accent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            radius: 6
                            color: addQBtn.hovered ? Qt.rgba(0, 0.71, 0.85, 0.15) : "transparent"
                            border.color: theme.accent
                            implicitWidth: 120
                            Behavior on color { ColorAnimation { duration: 120 } }
                        }
                        onClicked: addingQuestion = true
                    }
                }

                // ── Add question form ──
                Rectangle {
                    visible: addingQuestion
                    Layout.fillWidth: true
                    implicitHeight: addQCol.implicitHeight + 32
                    radius: 10; color: theme.surface
                    border.color: theme.accent
                    border.width: 1

                    ColumnLayout {
                        id: addQCol
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10

                        Label {
                            text: editingQuestionIndex >= 0 ? "Edit Question" : "New Question"
                            font { pixelSize: 13; weight: Font.DemiBold }
                            color: theme.textPrimary
                        }

                        // Title
                        TextField {
                            id: qTitle
                            Layout.fillWidth: true
                            placeholderText: "Question title"
                            color: theme.textPrimary
                            placeholderTextColor: theme.textMuted
                            font.pixelSize: 13
                            background: Rectangle {
                                radius: 6; color: theme.surfaceLight
                                border.color: qTitle.activeFocus ? theme.accent : theme.border
                            }
                        }

                        // Code snippet
                        TextField {
                            id: qCode
                            Layout.fillWidth: true
                            placeholderText: "Code snippet (optional)"
                            color: theme.textPrimary
                            placeholderTextColor: theme.textMuted
                            font { pixelSize: 13; family: "Consolas" }
                            enabled: qStrategy.currentIndex !== 1
                            opacity: enabled ? 1.0 : 0.4
                            background: Rectangle {
                                radius: 6; color: theme.codeBg
                                border.color: qCode.activeFocus ? theme.accent : theme.border
                            }
                        }

                        // Answer options (comma-separated)
                        ColumnLayout {
                            spacing: 4; Layout.fillWidth: true
                            Label { text: "Answer options (comma-separated)"; font.pixelSize: 11; color: theme.textSecondary }
                            TextField {
                                id: qOptions
                                Layout.fillWidth: true
                                placeholderText: "Option A, Option B, Option C, Option D"
                                color: theme.textPrimary
                                placeholderTextColor: theme.textMuted
                                font.pixelSize: 13
                                enabled: qStrategy.currentIndex === 0
                                opacity: enabled ? 1.0 : 0.4
                                background: Rectangle {
                                    radius: 6; color: theme.surfaceLight
                                    border.color: qOptions.activeFocus ? theme.accent : theme.border
                                }
                            }
                        }

                        // Correct answer
                        TextField {
                            id: qCorrect
                            Layout.fillWidth: true
                            placeholderText: "Correct answer"
                            color: theme.textPrimary
                            placeholderTextColor: theme.textMuted
                            font.pixelSize: 13
                            background: Rectangle {
                                radius: 6; color: theme.surfaceLight
                                border.color: qCorrect.activeFocus ? theme.accent : theme.border
                            }
                        }

                        // Explanation
                        TextField {
                            id: qExplanation
                            Layout.fillWidth: true
                            placeholderText: "Explanation (shown after answering)"
                            color: theme.textPrimary
                            placeholderTextColor: theme.textMuted
                            font.pixelSize: 13
                            background: Rectangle {
                                radius: 6; color: theme.surfaceLight
                                border.color: qExplanation.activeFocus ? theme.accent : theme.border
                            }
                        }

                        // Category
                        TextField {
                            id: qCategory
                            Layout.fillWidth: true
                            placeholderText: "Category (e.g. STL, Templates)"
                            color: theme.textPrimary
                            placeholderTextColor: theme.textMuted
                            font.pixelSize: 13
                            background: Rectangle {
                                radius: 6; color: theme.surfaceLight
                                border.color: qCategory.activeFocus ? theme.accent : theme.border
                            }
                        }

                        // Difficulty & Strategy row
                        RowLayout {
                            spacing: 12
                            Layout.fillWidth: true

                            ColumnLayout {
                                spacing: 4
                                Label { text: "Difficulty"; font.pixelSize: 11; color: theme.textSecondary }
                                ComboBox {
                                    id: qDifficulty
                                    model: ["Easy", "Medium", "Hard", "Expert"]
                                    Layout.preferredWidth: 140
                                    background: Rectangle {
                                        radius: 6; color: theme.surfaceLight
                                        border.color: theme.border
                                        implicitHeight: 32
                                    }
                                    contentItem: Label {
                                        text: qDifficulty.displayText
                                        font.pixelSize: 13; color: theme.textPrimary
                                        leftPadding: 8
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }

                            ColumnLayout {
                                spacing: 4
                                Label { text: "Answer Type"; font.pixelSize: 11; color: theme.textSecondary }
                                ComboBox {
                                    id: qStrategy
                                    model: ["Multiple Choice", "Text Input", "Code Output"]
                                    Layout.preferredWidth: 160
                                    background: Rectangle {
                                        radius: 6; color: theme.surfaceLight
                                        border.color: theme.border
                                        implicitHeight: 32
                                    }
                                    contentItem: Label {
                                        text: qStrategy.displayText
                                        font.pixelSize: 13; color: theme.textPrimary
                                        leftPadding: 8
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Form buttons
                        RowLayout {
                            Layout.topMargin: 4
                            spacing: 8

                            Button {
                                id: confirmAddBtn
                                Layout.preferredHeight: 32
                                enabled: qTitle.text.length > 0 && qCorrect.text.length > 0
                                contentItem: Label {
                                    text: editingQuestionIndex >= 0 ? "Update" : "Add"
                                    font { pixelSize: 12; weight: Font.DemiBold }
                                    color: confirmAddBtn.enabled ? "#ffffff" : theme.textMuted
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                background: Rectangle {
                                    radius: 6; implicitWidth: 70
                                    color: confirmAddBtn.enabled
                                        ? (confirmAddBtn.hovered ? theme.accentHover : theme.accent)
                                        : theme.surfaceLight
                                    Behavior on color { ColorAnimation { duration: 120 } }
                                }
                                onClicked: {
                                    var opts = qOptions.text.split(",").map(function(s) { return s.trim() }).filter(function(s) { return s.length > 0 })
                                    if (editingQuestionIndex >= 0) {
                                        createQuizVM.updateQuestion(
                                            editingQuestionIndex,
                                            qTitle.text, qCode.text, opts,
                                            qCorrect.text, qExplanation.text,
                                            qDifficulty.currentIndex, qStrategy.currentIndex,
                                            qCategory.text)
                                    } else {
                                        createQuizVM.addQuestion(
                                            qTitle.text, qCode.text, opts,
                                            qCorrect.text, qExplanation.text,
                                            qDifficulty.currentIndex, qStrategy.currentIndex,
                                            qCategory.text)
                                    }
                                    // Clear fields
                                    qTitle.text = ""; qCode.text = ""; qOptions.text = ""
                                    qCorrect.text = ""; qExplanation.text = ""; qCategory.text = ""
                                    qDifficulty.currentIndex = 0; qStrategy.currentIndex = 0
                                    addingQuestion = false
                                    editingQuestionIndex = -1
                                }
                            }

                            Button {
                                id: cancelAddBtn
                                Layout.preferredHeight: 32
                                contentItem: Label {
                                    text: "Cancel"
                                    font.pixelSize: 12
                                    color: theme.textSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                background: Rectangle {
                                    radius: 6; implicitWidth: 70
                                    color: cancelAddBtn.hovered ? theme.surfaceLight : "transparent"
                                    border.color: theme.border
                                    Behavior on color { ColorAnimation { duration: 120 } }
                                }
                                onClicked: { addingQuestion = false; editingQuestionIndex = -1 }
                            }
                        }
                    }
                }

                // ── Question cards ──
                Repeater {
                    model: createQuizVM.questionCount

                    Rectangle {
                        id: qCard
                        required property int index
                        Layout.fillWidth: true
                        height: 64; radius: 8
                        color: qCardArea.containsMouse ? theme.surfaceLight : theme.surface
                        border.color: qCardArea.containsMouse ? theme.border : Qt.rgba(0,0,0,0)
                        Behavior on color { ColorAnimation { duration: 120 } }

                        MouseArea {
                            id: qCardArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 12

                            // Index badge
                            Rectangle {
                                width: 28; height: 28; radius: 14
                                color: Qt.rgba(0, 0.71, 0.85, 0.1)
                                Label {
                                    anchors.centerIn: parent
                                    text: (qCard.index + 1)
                                    font { pixelSize: 12; weight: Font.Bold }
                                    color: theme.accent
                                }
                            }

                            ColumnLayout {
                                spacing: 2; Layout.fillWidth: true
                                Label {
                                    text: createQuizVM.questionTitleAt(qCard.index)
                                    font { pixelSize: 13; weight: Font.DemiBold }
                                    color: theme.textPrimary
                                    elide: Text.ElideRight; Layout.fillWidth: true
                                }
                                Label {
                                    text: createQuizVM.questionCategoryAt(qCard.index)
                                    font.pixelSize: 11
                                    color: theme.textSecondary
                                }
                            }

                            // Difficulty badge
                            Rectangle {
                                property var diffLabels: ["Easy", "Medium", "Hard", "Expert"]
                                property var diffColors: ["#00e676", "#ffb74d", "#ff5252", "#e040fb"]
                                property int diff: createQuizVM.questionDifficultyAt(qCard.index)
                                width: diffLabel.implicitWidth + 16
                                height: 22; radius: 11
                                color: Qt.rgba(0,0,0,0)
                                border.color: diffColors[diff]
                                Label {
                                    id: diffLabel
                                    anchors.centerIn: parent
                                    text: parent.diffLabels[parent.diff]
                                    font { pixelSize: 10; weight: Font.Medium }
                                    color: parent.diffColors[parent.diff]
                                }
                            }

                            // Edit question button
                            Button {
                                id: editQBtn
                                Layout.preferredWidth: 28; Layout.preferredHeight: 28
                                contentItem: Label {
                                    text: "✎"
                                    font.pixelSize: 12
                                    color: editQBtn.hovered ? theme.accent : theme.textMuted
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    Behavior on color { ColorAnimation { duration: 120 } }
                                }
                                background: Rectangle {
                                    radius: 6
                                    color: editQBtn.hovered ? Qt.rgba(0, 0.71, 0.85, 0.12) : "transparent"
                                    Behavior on color { ColorAnimation { duration: 120 } }
                                }
                                onClicked: {
                                    editingQuestionIndex = qCard.index
                                    qTitle.text = createQuizVM.questionTitleAt(qCard.index)
                                    qCode.text = createQuizVM.questionCodeSnippetAt(qCard.index)
                                    qOptions.text = createQuizVM.questionAnswerOptionsAt(qCard.index).join(", ")
                                    qCorrect.text = createQuizVM.questionCorrectAnswerAt(qCard.index)
                                    qExplanation.text = createQuizVM.questionExplanationAt(qCard.index)
                                    qCategory.text = createQuizVM.questionCategoryAt(qCard.index)
                                    qDifficulty.currentIndex = createQuizVM.questionDifficultyAt(qCard.index)
                                    qStrategy.currentIndex = createQuizVM.questionStrategyTypeAt(qCard.index)
                                    addingQuestion = true
                                }
                            }

                            // Remove button
                            Button {
                                id: removeBtn
                                Layout.preferredWidth: 28; Layout.preferredHeight: 28
                                contentItem: Label {
                                    text: "✕"
                                    font.pixelSize: 12
                                    color: removeBtn.hovered ? theme.error : theme.textMuted
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    Behavior on color { ColorAnimation { duration: 120 } }
                                }
                                background: Rectangle {
                                    radius: 6
                                    color: removeBtn.hovered ? theme.errorBg : "transparent"
                                    Behavior on color { ColorAnimation { duration: 120 } }
                                }
                                onClicked: createQuizVM.removeQuestion(qCard.index)
                            }
                        }
                    }
                }

                // Empty state
                Rectangle {
                    visible: createQuizVM.questionCount === 0 && !addingQuestion
                    Layout.fillWidth: true
                    height: 120; radius: 10
                    color: theme.surface
                    border.color: theme.border
                    border.width: 1

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Label {
                            text: "No questions yet"
                            font { pixelSize: 14; weight: Font.DemiBold }
                            color: theme.textMuted
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Label {
                            text: "Click \"+ Add Question\" to get started"
                            font.pixelSize: 12
                            color: theme.textMuted
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }

                // Bottom spacer
                Item { height: 20 }
            }
        }
    }

    Connections {
        target: createQuizVM
        function onQuizCreated() {
            createQuizPage.backClicked()
        }
    }
}

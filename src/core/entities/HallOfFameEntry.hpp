#pragma once
#include <QString>
#include <QDateTime>

namespace cowme::core {

struct HallOfFameEntry {
    int id{0};
    QString playerName;
    int quizId{0};
    QString quizName;
    int score{0};
    int totalQuestions{0};
    int elapsedSeconds{0};
    QDateTime timestamp;
};

} // namespace cowme::core

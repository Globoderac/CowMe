#pragma once
#include "DatabaseManager.hpp"
#include <QSqlQuery>
#include <QSqlError>

namespace cowme::infrastructure {

class DatabaseSeeder {
public:
    static bool seedIfEmpty(DatabaseManager& db) {
        QSqlQuery countQuery(db.database());
        if (!countQuery.exec("SELECT COUNT(*) FROM quizzes") || !countQuery.next())
            return false;

        if (countQuery.value(0).toInt() > 0)
            return true; // Already has data

        return insertSampleData(db);
    }

private:
    static bool insertSampleData(DatabaseManager& db) {
        QSqlDatabase& conn = db.database();
        conn.transaction();

        QSqlQuery q(conn);

        // Quiz 1: C++ Basics
        q.prepare("INSERT INTO quizzes (name, description) VALUES (?, ?)");
        q.addBindValue("C++ Basics");
        q.addBindValue("Fundamental C++ concepts — types, control flow, and basic syntax.");
        if (!q.exec()) { conn.rollback(); return false; }
        int64_t quiz1Id = q.lastInsertId().toLongLong();

        // Quiz 2: Modern C++ (C++17/20/23)
        q.prepare("INSERT INTO quizzes (name, description) VALUES (?, ?)");
        q.addBindValue("Modern C++ Features");
        q.addBindValue("Test your knowledge of C++17, C++20, and C++23 features.");
        if (!q.exec()) { conn.rollback(); return false; }
        int64_t quiz2Id = q.lastInsertId().toLongLong();

        // Quiz 3: OOP & Design Patterns
        q.prepare("INSERT INTO quizzes (name, description) VALUES (?, ?)");
        q.addBindValue("OOP & Design Patterns");
        q.addBindValue("Object-oriented programming principles and common design patterns in C++.");
        if (!q.exec()) { conn.rollback(); return false; }
        int64_t quiz3Id = q.lastInsertId().toLongLong();

        // --- Quiz 1 Questions ---
        insertQuestion(conn, quiz1Id,
            "What is the size of 'int' guaranteed to be in C++?",
            "",
            "At least 16 bits",
            "The C++ standard only guarantees int is at least 16 bits. "
            "On most modern platforms it is 32 bits, but that is implementation-defined.",
            0, 0, "Basics",
            {"Exactly 32 bits", "Exactly 16 bits", "At least 16 bits", "At least 8 bits"},
            {"types", "fundamentals"});

        insertQuestion(conn, quiz1Id,
            "Which keyword is used to prevent a class from being inherited?",
            "",
            "final",
            "The 'final' specifier (C++11) prevents a class from being used as a base class.",
            0, 0, "Basics",
            {"sealed", "final", "static", "const"},
            {"classes", "inheritance"});

        insertQuestion(conn, quiz1Id,
            "What does the following code print?",
            "int x = 5;\nstd::cout << x++ << \" \" << ++x;",
            "Undefined behavior",
            "Modifying a variable more than once between sequence points (before C++17) or "
            "having unsequenced side effects is undefined behavior.",
            1, 0, "Basics",
            {"5 7", "5 6", "6 7", "Undefined behavior"},
            {"operators", "UB"});

        insertQuestion(conn, quiz1Id,
            "What is the output?",
            "int a = 10;\nint& ref = a;\nref = 20;\nstd::cout << a;",
            "20",
            "A reference is an alias. Modifying ref changes a directly.",
            0, 0, "Basics",
            {"10", "20", "Compile error", "Undefined behavior"},
            {"references"});

        insertQuestion(conn, quiz1Id,
            "Which header provides std::vector?",
            "",
            "<vector>",
            "std::vector is declared in the <vector> header.",
            0, 0, "Basics",
            {"<array>", "<list>", "<vector>", "<container>"},
            {"STL", "containers"});

        // --- Quiz 2 Questions ---
        insertQuestion(conn, quiz2Id,
            "Which C++17 feature allows decomposing a struct into named variables?",
            "auto [x, y] = std::make_pair(1, 2);",
            "Structured bindings",
            "Structured bindings (C++17) let you bind names to elements of a pair, tuple, or aggregate.",
            1, 0, "Modern C++",
            {"Structured bindings", "Destructuring assignment", "Pattern matching", "Fold expressions"},
            {"C++17", "structured-bindings"});

        insertQuestion(conn, quiz2Id,
            "What does std::optional represent?",
            "",
            "A value that may or may not be present",
            "std::optional<T> (C++17) is a wrapper that may or may not hold a value of type T.",
            0, 0, "Modern C++",
            {"A thread-safe container", "A value that may or may not be present",
             "A smart pointer variant", "A compile-time constant"},
            {"C++17", "optional"});

        insertQuestion(conn, quiz2Id,
            "What C++20 feature constrains template parameters?",
            "template<std::integral T>\nT add(T a, T b) { return a + b; }",
            "Concepts",
            "Concepts (C++20) provide named constraints on template parameters.",
            1, 0, "Modern C++",
            {"Modules", "Coroutines", "Concepts", "Ranges"},
            {"C++20", "concepts"});

        insertQuestion(conn, quiz2Id,
            "What does std::expected (C++23) provide over std::optional?",
            "",
            "An error value when the expected value is absent",
            "std::expected<T,E> holds either a T value or an E error, "
            "unlike std::optional which only signals absence.",
            2, 0, "Modern C++",
            {"Thread safety", "An error value when the expected value is absent",
             "Automatic serialization", "Compile-time evaluation"},
            {"C++23", "expected"});

        insertQuestion(conn, quiz2Id,
            "What is the output?",
            "std::vector v{1, 2, 3, 4, 5};\nauto even = v | std::views::filter(\n"
            "    [](int n){ return n % 2 == 0; });\nfor (int n : even) std::cout << n << ' ';",
            "2 4",
            "std::views::filter (C++20 Ranges) lazily filters elements matching the predicate.",
            1, 2, "Modern C++",
            {},
            {"C++20", "ranges"});

        // --- Quiz 3 Questions ---
        insertQuestion(conn, quiz3Id,
            "Which OOP principle does the 'virtual' keyword enable?",
            "",
            "Polymorphism",
            "Virtual functions enable runtime polymorphism through dynamic dispatch.",
            0, 0, "OOP",
            {"Encapsulation", "Inheritance", "Polymorphism", "Abstraction"},
            {"OOP", "virtual"});

        insertQuestion(conn, quiz3Id,
            "What design pattern ensures only one instance of a class exists?",
            "",
            "Singleton",
            "The Singleton pattern restricts instantiation to a single object.",
            0, 0, "Design Patterns",
            {"Factory", "Singleton", "Observer", "Builder"},
            {"design-patterns", "singleton"});

        insertQuestion(conn, quiz3Id,
            "Which pattern is described: 'Define a family of algorithms, "
            "encapsulate each one, and make them interchangeable'?",
            "",
            "Strategy",
            "The Strategy pattern lets you swap algorithms at runtime without changing the client.",
            1, 0, "Design Patterns",
            {"Command", "Strategy", "State", "Template Method"},
            {"design-patterns", "strategy"});

        insertQuestion(conn, quiz3Id,
            "In C++, what is the Rule of Five?",
            "",
            "If you define any of destructor, copy/move constructor, or copy/move assignment, define all five",
            "The Rule of Five states that if a class needs a custom destructor, copy constructor, "
            "copy assignment, move constructor, or move assignment, it likely needs all five.",
            1, 1, "OOP",
            {},
            {"OOP", "rule-of-five"});

        insertQuestion(conn, quiz3Id,
            "What is the primary purpose of the CRTP (Curiously Recurring Template Pattern)?",
            "template<typename Derived>\nclass Base {\n    void interface() {\n"
            "        static_cast<Derived*>(this)->impl();\n    }\n};",
            "Static polymorphism",
            "CRTP achieves compile-time polymorphism by using the derived class "
            "as a template parameter of the base class.",
            2, 0, "Design Patterns",
            {"Runtime polymorphism", "Static polymorphism",
             "Memory management", "Thread safety"},
            {"design-patterns", "CRTP"});

        conn.commit();
        return true;
    }

    static void insertQuestion(
        QSqlDatabase& conn,
        int64_t quizId,
        const QString& title,
        const QString& codeSnippet,
        const QString& correctAnswer,
        const QString& explanation,
        int difficulty,
        int strategyType,
        const QString& category,
        const QStringList& options,
        const QStringList& tags)
    {
        QSqlQuery q(conn);
        q.prepare("INSERT INTO questions (quiz_id, title, code_snippet, correct_answer, "
                  "explanation, difficulty, strategy_type, category) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        q.addBindValue(quizId);
        q.addBindValue(title);
        q.addBindValue(codeSnippet);
        q.addBindValue(correctAnswer);
        q.addBindValue(explanation);
        q.addBindValue(difficulty);
        q.addBindValue(strategyType);
        q.addBindValue(category);
        if (!q.exec()) return;

        int64_t questionId = q.lastInsertId().toLongLong();

        for (const auto& opt : options) {
            QSqlQuery optQ(conn);
            optQ.prepare("INSERT INTO answer_options (question_id, option_text) VALUES (?, ?)");
            optQ.addBindValue(questionId);
            optQ.addBindValue(opt);
            optQ.exec();
        }

        for (const auto& tag : tags) {
            QSqlQuery tagQ(conn);
            tagQ.prepare("INSERT INTO question_tags (question_id, tag) VALUES (?, ?)");
            tagQ.addBindValue(questionId);
            tagQ.addBindValue(tag);
            tagQ.exec();
        }
    }
};

} // namespace cowme::infrastructure

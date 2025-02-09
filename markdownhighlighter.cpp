#include "markdownhighlighter.hpp"

MarkdownHighlighter::MarkdownHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Заголовки (начинаются с одного или нескольких символов #)
    headerFormat.setForeground(Qt::blue);
    headerFormat.setFontWeight(QFont::Bold);
    HighlightingRule rule;
    rule.pattern = QRegularExpression("^#{1,6} .+");
    rule.format = headerFormat;
    highlightingRules.append(rule);

    // Жирный: **текст**
    boldFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\*\\*[^\\*]+\\*\\*");
    rule.format = boldFormat;
    highlightingRules.append(rule);

    // Курсив: *текст*
    italicFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("\\*[^\\*]+\\*");
    rule.format = italicFormat;
    highlightingRules.append(rule);

    // Inline-код: `код`
    codeFormat.setFontFamily("Courier");
    codeFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("`[^`]+`");
    rule.format = codeFormat;
    highlightingRules.append(rule);
}

void MarkdownHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

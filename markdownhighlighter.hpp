#ifndef MARKDOWNHIGHLIGHTER_HPP
#define MARKDOWNHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class MarkdownHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit MarkdownHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat headerFormat;
    QTextCharFormat boldFormat;
    QTextCharFormat italicFormat;
    QTextCharFormat codeFormat;
};

#endif // MARKDOWNHIGHLIGHTER_HPP

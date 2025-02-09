#include "editortab.hpp"
#include "markdownhighlighter.hpp"

#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QScrollBar>
#include <QDir>

EditorTab::EditorTab(QWidget *parent)
    : QWidget(parent)
{
    splitter = new QSplitter(Qt::Horizontal, this);

    markdownEditor = new QPlainTextEdit(this);
    markdownEditor->setPlaceholderText("Введите Markdown текст...");
    previewBrowser = new QTextBrowser(this);

    splitter->addWidget(markdownEditor);
    splitter->addWidget(previewBrowser);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(splitter);
    setLayout(layout);

    highlighter = new MarkdownHighlighter(markdownEditor->document());

    connect(markdownEditor, &QPlainTextEdit::textChanged, this, &EditorTab::updatePreview);

    connect(markdownEditor->verticalScrollBar(), &QScrollBar::valueChanged,
            this, [this](int value) {
                QScrollBar *edBar = markdownEditor->verticalScrollBar();
                QScrollBar *prBar = previewBrowser->verticalScrollBar();
                int edMax = edBar->maximum();
                int prMax = prBar->maximum();
                if (edMax > 0 && prMax > 0) {
                    double ratio = static_cast<double>(value) / edMax;
                    prBar->setValue(static_cast<int>(ratio * prMax));
                }
            });

    updatePreview();
}

QPlainTextEdit* EditorTab::editor() const
{
    return markdownEditor;
}

QTextBrowser* EditorTab::preview() const
{
    return previewBrowser;
}

void EditorTab::updatePreview()
{
    QString markdownText = markdownEditor->toPlainText();
    QTextDocument doc;

    doc.setBaseUrl(QUrl::fromLocalFile(QDir::currentPath() + "/"));

    doc.setMarkdown(markdownText);
    QString html = doc.toHtml();

    QString style = "<style>"
                    "pre, code { font-family: 'Courier New', monospace; "
                    "background-color: #3d4345; padding: 5px; border-radius: 3px; }"
                    "</style>";
    html = style + html;

    previewBrowser->setHtml(html);
}

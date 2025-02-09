#ifndef EDITORTAB_HPP
#define EDITORTAB_HPP

#include <QWidget>

class QPlainTextEdit;
class QTextBrowser;
class QSplitter;
class MarkdownHighlighter;

class EditorTab : public QWidget
{
    Q_OBJECT
public:
    explicit EditorTab(QWidget *parent = nullptr);

    QPlainTextEdit* editor() const;
    QTextBrowser* preview() const;
    void updatePreview();

private:
    QPlainTextEdit *markdownEditor;
    QTextBrowser   *previewBrowser;
    QSplitter      *splitter;
    MarkdownHighlighter *highlighter;
};

#endif // EDITORTAB_HPP

#include "mainwindow.hpp"
#include "editortab.hpp"
#include <QPlainTextEdit>
#include <QTextBrowser>

#include <QUrl>
#include <QApplication>
#include <QTabWidget>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QProcess>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    newTab();

    createActions();
    createMenus();
    createToolBar();
    setupAutoSave();
}

///////////////////////////
// Создание действий
///////////////////////////

void MainWindow::createActions()
{
    // Файловые операции
    newTabAct = new QAction(tr("Новая вкладка"), this);
    connect(newTabAct, &QAction::triggered, this, &MainWindow::newTab);

    openAct = new QAction(tr("Открыть..."), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    saveAct = new QAction(tr("Сохранить"), this);
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    saveAsAct = new QAction(tr("Сохранить как..."), this);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFileAs);

    exportPdfAct = new QAction(tr("Экспорт в PDF"), this);
    connect(exportPdfAct, &QAction::triggered, this, &MainWindow::exportToPdf);

    // Форматирование
    boldAct = new QAction(tr("Жирный"), this);
    connect(boldAct, &QAction::triggered, this, &MainWindow::makeBold);

    italicAct = new QAction(tr("Курсив"), this);
    connect(italicAct, &QAction::triggered, this, &MainWindow::makeItalic);

    underlineAct = new QAction(tr("Подчеркнутый"), this);
    connect(underlineAct, &QAction::triggered, this, &MainWindow::makeUnderline);

    lineBreakAct = new QAction(tr("Перенос строки (<br/>)"), this);
    connect(lineBreakAct, &QAction::triggered, this, &MainWindow::insertLineBreak);

    headingAct = new QAction(tr("Заголовок"), this);
    connect(headingAct, &QAction::triggered, this, &MainWindow::insertHeading);

    bulletListAct = new QAction(tr("Маркированный список"), this);
    connect(bulletListAct, &QAction::triggered, this, &MainWindow::insertBulletList);

    numberedListAct = new QAction(tr("Нумерованный список"), this);
    connect(numberedListAct, &QAction::triggered, this, &MainWindow::insertNumberedList);

    quoteAct = new QAction(tr("Цитата"), this);
    connect(quoteAct, &QAction::triggered, this, &MainWindow::insertQuote);

    codeBlockAct = new QAction(tr("Блок кода"), this);
    connect(codeBlockAct, &QAction::triggered, this, &MainWindow::insertCodeBlock);

    // Прочие функции
    toggleThemeAct = new QAction(tr("Переключить тему"), this);
    connect(toggleThemeAct, &QAction::triggered, this, &MainWindow::toggleTheme);

    helpAct = new QAction(tr("Markdown справка"), this);
    connect(helpAct, &QAction::triggered, this, &MainWindow::showMarkdownHelp);

    insertImageAct = new QAction(tr("Вставить изображение"), this);
    connect(insertImageAct, &QAction::triggered, this, &MainWindow::insertImage);

    gitStatusAct = new QAction(tr("Git Status"), this);
    connect(gitStatusAct, &QAction::triggered, this, &MainWindow::gitStatus);
}

///////////////////////////
// Создание меню и панели
///////////////////////////

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(newTabAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exportPdfAct);

    editMenu = menuBar()->addMenu(tr("Правка"));
    editMenu->addAction(boldAct);
    editMenu->addAction(italicAct);
    editMenu->addAction(underlineAct);
    editMenu->addAction(lineBreakAct);
    editMenu->addSeparator();
    editMenu->addAction(headingAct);
    editMenu->addAction(bulletListAct);
    editMenu->addAction(numberedListAct);
    editMenu->addAction(quoteAct);
    editMenu->addAction(codeBlockAct);
    editMenu->addSeparator();
    editMenu->addAction(insertImageAct);

    viewMenu = menuBar()->addMenu(tr("Вид"));
    viewMenu->addAction(toggleThemeAct);

    helpMenu = menuBar()->addMenu(tr("Справка"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(gitStatusAct);
}

void MainWindow::createToolBar()
{
    formatToolBar = addToolBar(tr("Форматирование"));
    formatToolBar->addAction(boldAct);
    formatToolBar->addAction(italicAct);
    formatToolBar->addAction(underlineAct);
    formatToolBar->addSeparator();
    formatToolBar->addAction(headingAct);
    formatToolBar->addAction(bulletListAct);
    formatToolBar->addAction(numberedListAct);
    formatToolBar->addAction(quoteAct);
    formatToolBar->addAction(codeBlockAct);
    formatToolBar->addSeparator();
    formatToolBar->addAction(lineBreakAct);
    formatToolBar->addSeparator();
    formatToolBar->addAction(insertImageAct);
}

///////////////////////////
// Действия файловых операций
///////////////////////////

void MainWindow::newTab()
{
    EditorTab *tab = new EditorTab;
    int index = tabWidget->addTab(tab, tr("Без имени"));
    tabWidget->setCurrentIndex(index);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Открыть Markdown файл"),
                                                    "",
                                                    tr("Markdown Files (*.md);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не могу открыть файл для чтения."));
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    EditorTab *tab = new EditorTab;
    tab->editor()->setPlainText(text);
    int index = tabWidget->addTab(tab, QFileInfo(fileName).fileName());
    tabWidget->setCurrentIndex(index);
}

void MainWindow::saveFile()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить Markdown файл"),
                                                    "",
                                                    tr("Markdown Files (*.md);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не могу сохранить файл."));
        return;
    }
    QTextStream out(&file);
    out << tab->editor()->toPlainText();
    file.close();
    tabWidget->setTabText(tabWidget->currentIndex(), QFileInfo(fileName).fileName());
}

void MainWindow::saveFileAs()
{
    saveFile();
}

void MainWindow::exportToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Экспорт в PDF"),
                                                    "",
                                                    tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty())
        return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (tab)
        tab->preview()->document()->print(&printer);
}

///////////////////////////
// Действия форматирования
///////////////////////////

void MainWindow::makeBold()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    if (!cursor.hasSelection()) {
        cursor.insertText("****");
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);
    } else {
        QString text = cursor.selectedText();
        cursor.insertText("**" + text + "**");
    }
}

void MainWindow::makeItalic()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    if (!cursor.hasSelection()) {
        cursor.insertText("**");
        cursor.movePosition(QTextCursor::Left);
    } else {
        QString text = cursor.selectedText();
        cursor.insertText("*" + text + "*");
    }
}

void MainWindow::makeUnderline()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    if (!cursor.hasSelection()) {
        cursor.insertText("<u></u>");
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 4);
    } else {
        QString text = cursor.selectedText();
        cursor.insertText("<u>" + text + "</u>");
    }
}

void MainWindow::insertLineBreak()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    cursor.insertText("<br/>");
}

void MainWindow::insertHeading()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.insertText("# ");
}

void MainWindow::insertBulletList()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.insertText("- ");
}

void MainWindow::insertNumberedList()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.insertText("1. ");
}

void MainWindow::insertQuote()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.insertText("> ");
}

void MainWindow::insertCodeBlock()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QTextCursor cursor = tab->editor()->textCursor();
    cursor.insertText("```\n\n```\n");
    cursor.movePosition(QTextCursor::Up);
}

///////////////////////////
// Дополнительные функции
///////////////////////////

void MainWindow::toggleTheme()
{
    static bool dark = false;
    if (!dark) {
        static_cast<QApplication*>(qApp)->setStyleSheet(
            "QMainWindow { background-color: #2b2b2b; }"
            "QTabWidget::pane { background-color: #2b2b2b; border: 1px solid #444444; }"
            "QTabBar::tab { background: #3c3c3c; color: #dcdcdc; padding: 5px; border: 1px solid #444444; }"
            "QTabBar::tab:selected { background: #555555; }"
            "QPlainTextEdit, QTextBrowser { background-color: #1e1e1e; color: #dcdcdc; border: none; }"
            "QToolBar, QMenuBar, QMenu { background-color: #3c3c3c; color: #dcdcdc; }"
            "QScrollBar { background-color: #2b2b2b; }"
            );
    } else {
        static_cast<QApplication*>(qApp)->setStyleSheet("");
    }
    dark = !dark;
}


void MainWindow::showMarkdownHelp()
{
    QString helpText =
        "<h2>Markdown Справка</h2>"
        "<p><strong>Заголовки:</strong> Начинайте строку с символов <code>#</code> "
        "(например, <code># Заголовок 1</code>, <code>## Заголовок 2</code> и т.д.).</p>"
        "<p><strong>Жирный текст:</strong> Обрамляйте текст двойными звездочками: <code>**текст**</code>.</p>"
        "<p><strong>Курсив:</strong> Обрамляйте текст одинарными звездочками: <code>*текст*</code>.</p>"
        "<p><strong>Код:</strong> Для inline-кода используйте обратные кавычки: <code>`код`</code>, "
        "а для блоков кода --- тройные обратные кавычки:</p>"
        "<pre>```\nкод\n```</pre>"
        "<p><strong>Цитаты:</strong> Начинайте строку с символа <code>></code> для цитаты.</p>"
        "<p><strong>Изображения:</strong> Вставляйте их по схеме: <code>![альтернативный текст](URL)</code>.</p>"
        "<p>Дополнительную информацию можно найти на <a href='https://commonmark.org/help/'>CommonMark Help</a>.</p>";

    QMessageBox::information(this, tr("Markdown Справка"), helpText);
}


void MainWindow::insertImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Выбрать изображение"),
                                                    "",
                                                    tr("Image Files (*.png *.jpg *.jpeg *.gif);;All Files (*)"));
    if (!fileName.isEmpty()) {
        // Преобразуем локальный путь в QUrl
        QUrl url = QUrl::fromLocalFile(fileName);
        QString escapedUrl = url.toString(QUrl::FullyEncoded);

        // Создаем Markdown-строку для вставки изображения
        QString syntax = QString("![alt text](%1)").arg(escapedUrl);
        EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
        if (!tab)
            return;
        QTextCursor cursor = tab->editor()->textCursor();
        cursor.insertText(syntax);
    }
}

void MainWindow::gitStatus()
{
    EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->currentWidget());
    if (!tab)
        return;
    QString repoPath = QFileDialog::getExistingDirectory(this, tr("Выберите каталог Git репозитория"));
    if (repoPath.isEmpty())
        return;
    QProcess process;
    process.setWorkingDirectory(repoPath);
    process.start("git", QStringList() << "status");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QMessageBox::information(this, tr("Git Status"), output);
}

///////////////////////////
// Автосохранение
///////////////////////////

void MainWindow::setupAutoSave()
{
    autosaveTimer = new QTimer(this);
    connect(autosaveTimer, &QTimer::timeout, this, &MainWindow::autosaveAllTabs);
    autosaveTimer->start(300000); // каждые 5 минут
}

void MainWindow::autosaveAllTabs()
{
    int count = tabWidget->count();
    for (int i = 0; i < count; ++i) {
        EditorTab *tab = qobject_cast<EditorTab*>(tabWidget->widget(i));
        if (!tab)
            continue;
        QString backupDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(backupDir);
        QString backupFile = backupDir + "/backup_tab_" + QString::number(i) + ".md";
        QFile file(backupFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << tab->editor()->toPlainText();
            file.close();
        }
    }
}

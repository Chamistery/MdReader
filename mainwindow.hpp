#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class QTabWidget;
class QAction;
class QMenu;
class QToolBar;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // Файловые операции
    void newTab();
    void openFile();
    void saveFile();
    void saveFileAs();
    void exportToPdf();

    // Форматирование
    void makeBold();
    void makeItalic();
    void makeUnderline();
    void insertLineBreak();
    void insertHeading();
    void insertBulletList();
    void insertNumberedList();
    void insertQuote();
    void insertCodeBlock();

    // Дополнительные функции
    void toggleTheme();
    void showMarkdownHelp();
    void insertImage();
    void gitStatus();
    void autosaveAllTabs();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void setupAutoSave();

    QTabWidget *tabWidget;

    // Файловые действия
    QAction *newTabAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportPdfAct;

    // Форматирование
    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;
    QAction *lineBreakAct;
    QAction *headingAct;
    QAction *bulletListAct;
    QAction *numberedListAct;
    QAction *quoteAct;
    QAction *codeBlockAct;

    // Прочие
    QAction *toggleThemeAct;
    QAction *helpAct;
    QAction *insertImageAct;
    QAction *gitStatusAct;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *formatToolBar;

    // Для автосохранения
    QTimer *autosaveTimer;
};

#endif // MAINWINDOW_HPP

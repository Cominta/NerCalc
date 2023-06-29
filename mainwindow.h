#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "parser.h"
#include <QTextBlock>
#include <QKeyEvent>
#include <QMenuBar>
#include <cmath>
#include <QInputDialog>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct ExprConfig
{
    bool spacingAfterEqual;
    int precision;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        Ui::MainWindow *ui;
        Parser* mathParser;
        ExprConfig* config;

        int fontSize;

    private slots:
        void editRow();
        void refillEntry(QStringList Qstr, int currentRow, int column, bool enter = false);
        void enter(bool enter, bool wrongEnter = false);
        QString equal(QString string);
        void var(QString string);
//        void deleteEqual(bool equal, int oldSizeSet = -1);
        void setFontSize(QTextCursor& tmpCursor);

        void on_actionFont_size_triggered();

        void on_actionInterval_triggered();

        void on_actionTrue_triggered();

        void on_actionFalse_triggered();

        void on_actionSet_precision_triggered();

public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        bool eventFilter(QObject* obj, QEvent* event);
};

#endif // MAINWINDOW_H

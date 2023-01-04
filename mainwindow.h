#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mathParser.h"
#include <QKeyEvent>
#include <QTextBlock>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        Ui::MainWindow *ui;
        MathParser* mathParser;

    private slots:
        void editRow();
        void refillEntry(QStringList Qstr, int currentRow);
        void enter(bool enter);
        QString equal(QString string);
        void var(QString string);

public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        bool eventFilter(QObject* obj, QEvent* event);
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    this->mathParser = new MathParser();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enter(bool enter)
{
    int currentRow = ui->entry->textCursor().blockNumber() - (enter ? 1 : 0);
    QStringList Qstr = ui->entry->toPlainText().split("\n");
    QString result;

    if (isalpha(Qstr[currentRow].toStdString()[0]) && Qstr[currentRow].replace(" ", "").indexOf("=") == 1)
    {
        if (Qstr[currentRow].size() > 2)
        {
            this->var(Qstr[currentRow]);
        }

        else
        {
            return;
        }
    }

    else
    {
        if (Qstr[currentRow].contains("="))
        {
            Qstr[currentRow] = Qstr[currentRow].split("=")[0];
            Qstr[currentRow].replace("=", "");
        }

        Qstr[currentRow].replace(" ", "");

        result = this->equal(Qstr[currentRow]);
        Qstr[currentRow] = result;
        refillEntry(Qstr, currentRow);
    }
}

QString MainWindow::equal(QString string)
{
    if (string.contains('='))
    {
        string = string.split('=')[0];
    }

    // Qstr[currentRow].toStdString()
    double result;
    bool notError = true;

    try
    {
        result = this->mathParser->parseString(string.toStdString());
    }

    catch (int)
    {
        notError = false;
    }

    if (notError)
    {
        QString equalStr("=" + QString::number(result));
        string += equalStr;
    }

    return string;
}

void MainWindow::editRow()
{
    int currentRow = ui->entry->textCursor().blockNumber();
    int lastRow = ui->entry->textCursor().blockNumber() - 1;
    QStringList Qstr =  ui->entry->toPlainText().split("\n");

    if (isalpha(Qstr[lastRow].toStdString()[0]) && Qstr[lastRow].replace(" ", "").indexOf("=") == 1)
    {
        Qstr[currentRow] = Qstr[lastRow][0] + Qstr[currentRow];
    }

    else
    {
        Qstr[currentRow] = Qstr[lastRow].split("=")[1] + Qstr[currentRow];
    }

    this->refillEntry(Qstr, currentRow);
}

void MainWindow::refillEntry(QStringList Qstr, int currentRow)
{
    ui->entry->clear();

    for (int i = 0; i < Qstr.length(); i++)
    {
        ui->entry->append(Qstr[i]);
    }

    QTextCursor tmpCursor = ui->entry->textCursor();
    tmpCursor.movePosition(QTextCursor::Start);
    tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveMode::MoveAnchor, currentRow);
    tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveMode::MoveAnchor, QTextCursor::MoveOperation::End);
    ui->entry->setTextCursor(tmpCursor);

//    ui->entry->insertPlainText(Qstr[currentRow]);
}

void MainWindow::var(QString string)
{
    std::string stdStr = string.toStdString();

    char name = stdStr[0];
    std::string value = stdStr.substr(2);

    this->mathParser->addVariable(name, value);
}


bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    static int count = -1;

    if (event->type() == QKeyEvent::KeyRelease)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        if(key->key() == Qt::Key_Return)
        {
            if (count == -1)
            {
                this->enter(true);
            }

            count++;
        }

        else if ((key->key() == Qt::Key_Plus || key->key() == Qt::Key_Minus || key->key() == Qt::Key_Backslash || key->key() == Qt::Key_Asterisk) &&
                 ui->entry->toPlainText().split("\n")[ui->entry->textCursor().blockNumber()].size() == 1 && ui->entry->textCursor().blockNumber() != 0)
        {
            if (count == -1)
            {
                this->editRow();
            }

            count++;
        }

        else if (key->key() == Qt::Key_Equal)
        {
            if (count == -1)
            {
                this->enter(false);
            }

            count++;
        }

        if (count == 3)
        {
            count = -1;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

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

void MainWindow::enter(bool enter, bool wrongEnter)
{
    int currentRow = ui->entry->textCursor().blockNumber() - (enter ? 1 : 0);
    QStringList Qstr = ui->entry->toPlainText().split("\n");
    QString result;

    if (isalpha(Qstr[currentRow].toStdString()[0]) && Qstr[currentRow].replace(" ", "").indexOf("=") == 1)
    {
        if (enter && Qstr[currentRow].size() == 2)
        {
            QString stringOut = Qstr[currentRow];

            stringOut += QString::number(this->mathParser->getValueVariable(Qstr[currentRow][0].toLatin1()));
            Qstr[currentRow] = stringOut;

            this->refillEntry(Qstr, currentRow, ui->entry->textCursor().columnNumber());
        }

        else if (Qstr[currentRow].size() > 2)
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
        QString toCalc = Qstr[currentRow];

        if (wrongEnter)
        {
            toCalc += Qstr[currentRow + 1];
            Qstr[currentRow + 1] = "";
        }

        if (toCalc.contains('='))
        {
            toCalc = toCalc.split("=")[0];
            toCalc.replace("=", "");
        }

        toCalc.replace(" ", "");

        result = this->equal(toCalc);
        Qstr[currentRow] = result;

        if (enter && currentRow < Qstr.size() - 1)
        {
//            currentRow--;
        }

        this->refillEntry(Qstr, (enter ? currentRow + 1 : currentRow), ui->entry->textCursor().columnNumber() + result.size());
    }
}

QString MainWindow::equal(QString string)
{
//    if (string.contains('='))
//    {
//        string = string.split('=')[0];
//    }

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

    this->refillEntry(Qstr, currentRow, ui->entry->textCursor().columnNumber() + Qstr[lastRow].split("=")[1].size());
}

void MainWindow::refillEntry(QStringList Qstr, int currentRow, int column)
{
    ui->entry->clear();

    for (int i = 0; i < Qstr.length(); i++)
    {
        ui->entry->append(Qstr[i]);
    }

    QTextCursor tmpCursor = ui->entry->textCursor();
    tmpCursor.movePosition(QTextCursor::Start);
    tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveMode::MoveAnchor, currentRow);
    tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveMode::MoveAnchor, column);
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

void MainWindow::deleteEqual(bool equal, int oldSizeSet)
{
    static int oldSize = 0;
    static int oldRow = 0;

    if (oldSizeSet != -1)
    {
        oldSize = oldSizeSet;
        return;
    }

    int currentRow = ui->entry->textCursor().blockNumber();
    QStringList Qstr =  ui->entry->toPlainText().split("\n");
    int currentSize = Qstr[currentRow].size();

    if (currentRow != oldRow)
    {
        oldSize = currentSize;
        oldRow = currentRow;
    }

    if (Qstr[currentRow].contains("=") && currentSize != oldSize && !equal)
    {
        Qstr[currentRow] = Qstr[currentRow].split("=")[0];
        this->refillEntry(Qstr, currentRow, ui->entry->textCursor().columnNumber());

        oldSize = currentSize;
        oldRow = currentRow;
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    static int count = -1;

    if (event->type() == QKeyEvent::KeyRelease)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        bool equal = false;

        if(key->key() == Qt::Key_Return)
        {
            if (count == -1)
            {
                if (ui->entry->textCursor().columnNumber() != ui->entry->toPlainText().split("\n")[ui->entry->textCursor().blockNumber()].size())
                {
                    this->enter(true, true);
                }

                else
                {
                    this->enter(true);
                }

                equal = true;
            }
        }

        else if ((key->key() == Qt::Key_Plus || key->key() == Qt::Key_Minus || key->key() == Qt::Key_Slash || key->key() == Qt::Key_Asterisk) &&
                 ui->entry->toPlainText().split("\n")[ui->entry->textCursor().blockNumber()].size() == 1 && ui->entry->textCursor().blockNumber() != 0)
        {
            if (count == -1)
            {
                this->editRow();
            }
        }

        else if (key->key() == Qt::Key_Equal)
        {
            if (count == -1)
            {
                this->enter(false);
                equal = true;
            }
        }

        if (count == -1 /*&& key->key() == Qt::Key_Backspace*/ &&
            !isalpha(ui->entry->toPlainText().split("\n")[ui->entry->textCursor().blockNumber()].toStdString()[0]) &&
            !(ui->entry->toPlainText().split("\n")[ui->entry->textCursor().blockNumber()].toStdString()[1] == '='))
        {
            this->deleteEqual(equal);
        }

        count++;

        if (count == 3)
        {
            count = -1;
        }

        this->deleteEqual(false, ui->entry->toPlainText().split("\n")[ui->entry->textCursor().blockNumber()].size());
    }

    return QMainWindow::eventFilter(obj, event);
}

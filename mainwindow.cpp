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

    this->fontSize = ui->entry->font().pointSize();
    this->mathParser = new MathParser();

    QMenuBar* menubar = new QMenuBar(nullptr);
    menubar->addMenu("Settings");

//    ui->entry->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->mathParser;
}

void MainWindow::enter(bool enter, bool wrongEnter)
{
    int currentRow = ui->entry->textCursor().blockNumber() - (enter ? 1 : 0);
    QStringList Qstr = ui->entry->toPlainText().split("\n");
    QString result;

    if (Qstr.size() == 0 || Qstr[0].size() == 0 || currentRow < 0)
    {
        return;
    }

    if ((Qstr[currentRow].size() != 0 && Qstr[currentRow][0] == '#'))
    {
        this->refillEntry(Qstr, (enter ? currentRow + 1 : currentRow), ui->entry->textCursor().columnNumber() + result.size(), true);
        return;
    }

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

        this->refillEntry(Qstr, (enter ? currentRow + 1 : currentRow), ui->entry->textCursor().columnNumber() + result.size(), true);
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

        if (std::isinf(result))
        {
            MessageBox(nullptr, TEXT("Number is inf"), TEXT("ERROR"), MB_OK);
            notError = false;
        }

        else if (std::isnan(result))
        {
            MessageBox(nullptr, TEXT("Not a number"), TEXT("ERROR"), MB_OK);
            notError = false;
        }
    }

    catch (int)
    {
        notError = false;
    }

    if (notError)
    {
//        int countNumbers = 0;
//        int num = result;

//        while (num != 0)
//        {
//            countNumbers++;
//            num -= num % 10;
//            num /= 10;
//        }

        QString equalStr;
        equalStr.setNum(std::round(result * 100) / 100/*, 'g', this->maxNumbers*/);
        equalStr.insert(0, '=');

        string += equalStr;
    }

    return string;
}

void MainWindow::editRow()
{
    int currentRow = ui->entry->textCursor().blockNumber();
    int lastRow = ui->entry->textCursor().blockNumber() - 1;
    QStringList Qstr =  ui->entry->toPlainText().split("\n");

    if (Qstr[lastRow].size() == 0 || Qstr[lastRow][0] == '#')
    {
        return;
    }

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

void MainWindow::setFontSize(QTextCursor& tmpCursor)
{
    ui->entry->selectAll();
    ui->entry->setFontPointSize(this->fontSize);
}

void MainWindow::refillEntry(QStringList Qstr, int currentRow, int column, bool enter)
{
    ui->entry->clear();

    for (int i = 0; i < Qstr.length(); i++)
    {
        QTextCharFormat format;

        if (Qstr[i].size() != 0 && Qstr[i][0] == '#')
        {
            ui->entry->append(QString("<font color=green>%1<\\font>").arg(Qstr[i]));
        }

        else
        {
            ui->entry->append(QString("<font color=black margin=10px>%1<\\font>").arg(Qstr[i]));
        }
    }

    QTextCursor tmpCursor = ui->entry->textCursor();
    tmpCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, currentRow);
    tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column);
    this->setFontSize(tmpCursor);
    QFont font = ui->entry->font();
    ui->entry->setTextCursor(tmpCursor);
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

    if (Qstr[currentRow].size() != 0 && Qstr[currentRow][0] != '#' && Qstr[currentRow].contains("=") && currentSize != oldSize && !equal)
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

        try
        {
            if(key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
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

        catch (const std::exception& ex)
        {
            MessageBox(nullptr, TEXT("ERROR"), TEXT("ERROR"), MB_OK);
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_actionFont_size_triggered()
{
    this->fontSize = QInputDialog::getInt(this, "Font size", "Enter font size:", this->fontSize, 0, 100);
    QTextCursor tmpCursor = ui->entry->textCursor();
    this->setFontSize(tmpCursor);
    ui->entry->setTextCursor(tmpCursor);
}


void MainWindow::on_actionInterval_triggered()
{
    int spacing = QInputDialog::getInt(this, "Letter spacing", "Enter letter spacing:", 0, 0, 10);
    ui->entry->setStyleSheet(("QTextEdit {letter-spacing: " + std::to_string(spacing) + "px;}").data());
}

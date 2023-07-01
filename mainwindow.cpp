#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <iostream>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    this->ui->actionTrue->setCheckable(true);
    this->ui->actionFalse->setCheckable(true);
    this->ui->actionTrue->setChecked(true);

    this->fontSize = ui->entry->font().pointSize();
    this->mathParser = new Parser();

    this->config = new ExprConfig();
    this->config->spacingAfterEqual = true;
    this->config->precision = 4;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->mathParser;
    delete this->config;
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

            stringOut += QString::number(this->mathParser->getValVar(Qstr[currentRow][0].toLatin1()));
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
            Qstr.erase(Qstr.begin() + currentRow);
//            Qstr[currentRow + 1] = "";
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
    QString result;
    bool notError = true;

    try
    {
        double resultDouble = this->mathParser->parse(string.toStdString());
        char format = 'f';
        int precision = this->config->precision;

        if (this->mathParser->getExp())
        {
            format = 'e';
        }

        result = QString::number(resultDouble, format, precision);

        if (!this->mathParser->getExp())
        {
            result.remove(QRegularExpression("0+$"));
            result.remove(QRegularExpression("\\.$"));
        }

        else
        {
            // delete zeros
            QString toEditLeft = result.split('e')[0];
            int i = toEditLeft.size() - 1;

            qDebug() << (toEditLeft[i] == 0) << " " << (toEditLeft[i]);

            while (i >= 0 && toEditLeft[i] == '0')
            {
                toEditLeft.remove(i, 1);
                i--;
            }

            if (toEditLeft[i] == '.')
            {
               toEditLeft.remove(i, 1);
            }

            result = toEditLeft + 'e' + result.split('e')[1];
        }

        if (std::isinf(result.toDouble()))
        {
            MessageBox(nullptr, TEXT("Number is inf"), TEXT("ERROR"), MB_OK);
            notError = false;
        }

        else if (std::isnan(result.toDouble()))
        {
            MessageBox(nullptr, TEXT("Not a number"), TEXT("ERROR"), MB_OK);
            notError = false;
        }
    }

    catch (EmptyLine& empty)
    {
        return "";
    }

    catch (std::exception& exp)
    {
        std::string what = exp.what();
//        char buff[20];
//        snprintf(buff, sizeof buff, "%s", what.c_str());
        MessageBoxA(nullptr, what.c_str(), "ERROR", MB_OK);
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
        equalStr += result;

        if (this->config->spacingAfterEqual)
        {
            equalStr.push_front(" = ");
        }

        else
        {
            equalStr.insert(0, '=');
        }

        string += equalStr;
    }

    return string;
}

void MainWindow::editRow()
{
    int currentRow = ui->entry->textCursor().blockNumber();
    int lastRow = ui->entry->textCursor().blockNumber() - 1;
    QStringList Qstr =  ui->entry->toPlainText().split("\n");
    bool var = false;

    if (Qstr[lastRow].size() == 0 || Qstr[lastRow][0] == '#')
    {
        return;
    }

    if (isalpha(Qstr[lastRow].toStdString()[0]) && Qstr[lastRow].replace(" ", "").indexOf("=") == 1)
    {
        var = true;
        Qstr[currentRow] = Qstr[lastRow][0] + Qstr[currentRow];
    }

    else if (Qstr[lastRow].split("=").size() > 1)
    {
        Qstr[currentRow] = Qstr[lastRow].split("=")[1] + Qstr[currentRow];
    }

    else
    {
        return;
    }

    this->refillEntry(Qstr, currentRow + 1, ui->entry->textCursor().columnNumber() + Qstr[lastRow].split("=")[1].size(), true);

    QTextCursor tmpCursor = ui->entry->textCursor();
    int cursorPos = tmpCursor.position();

    if (var)
    {
        tmpCursor.setPosition(cursorPos + Qstr[lastRow].split("=")[0].replace(" ", "").size());
    }

    else
    {
        tmpCursor.setPosition(cursorPos + Qstr[lastRow].split("=")[1].replace(" ", "").size());
    }

    ui->entry->setTextCursor(tmpCursor);
}

void MainWindow::setFontSize(QTextCursor& tmpCursor)
{
    ui->entry->selectAll();
    ui->entry->setFontPointSize(this->fontSize);
}

void MainWindow::refillEntry(QStringList Qstr, int currentRow, int column, bool enter)
{
    QTextCursor tmpCursor = ui->entry->textCursor();
    int cursorPos = tmpCursor.position();
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

    tmpCursor.setPosition(cursorPos);

    this->setFontSize(tmpCursor);
    QFont font = ui->entry->font();
    ui->entry->setTextCursor(tmpCursor);
}

void MainWindow::var(QString string)
{
    std::string stdStr = string.toStdString();

    char name = stdStr[0];
    std::string value = stdStr.substr(2);

    this->mathParser->setVar(name, this->mathParser->parse(value));
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

        if (ui->entry->textCursor().columnNumber() > Qstr[currentRow].indexOf('='))
        {
            this->refillEntry(Qstr, currentRow, ui->entry->textCursor().columnNumber() - Qstr[currentRow].split("=")[0].size());
        }

        else
        {
            this->refillEntry(Qstr, currentRow, ui->entry->textCursor().columnNumber());
        }

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
            if (count == -1)
            {
                std::string what = ex.what();
                MessageBoxA(nullptr, what.c_str(), "ERROR", MB_OK);
            }

            count++;
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


void MainWindow::on_actionTrue_triggered()
{
    if (this->config->spacingAfterEqual)
    {
        this->ui->actionTrue->setChecked(true);
        return;
    }

    else if (!this->config->spacingAfterEqual)
    {
        this->config->spacingAfterEqual = true;
        this->ui->actionFalse->setChecked(false);
    }
}


void MainWindow::on_actionFalse_triggered()
{
    if (!this->config->spacingAfterEqual)
    {
        this->ui->actionFalse->setChecked(true);
        return;
    }

    else if (this->config->spacingAfterEqual)
    {
        this->config->spacingAfterEqual = false;
        this->ui->actionTrue->setChecked(false);
    }
}

void MainWindow::on_actionSet_precision_triggered()
{
    this->config->precision = QInputDialog::getInt(this, "Precision", "Enter precision:", this->config->precision, 1, 20);
}

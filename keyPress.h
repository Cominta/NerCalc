#ifndef KEYPRESS_H
#define KEYPRESS_H

#include <QTextEdit>
#include <QKeyEvent>

class KeyPress : public QTextEdit
{
    Q_OBJECT
    public:
        void keyPressEvent(QKeyEvent* event);
};

#endif // KEYPRESS_H

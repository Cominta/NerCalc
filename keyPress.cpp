#include "keyPress.h"
#include <iostream>

void KeyPress::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter)
    {
        std::cout << "W";
    }

    else
    {
        QTextEdit::keyPressEvent(event);
    }
}

#include "mytextedit.h"
#include <iostream>
MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        if(e->modifiers() != Qt::ControlModifier)
        {
            QString text = toPlainText();
            clear();
            emit signalNewText(text);
        }
        else
        {
            QTextEdit::append("");
        }
    }
    else
    {
         QTextEdit::keyPressEvent( e );
    }
}

#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>
#include <QString>
#include <QKeyEvent>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *e);

signals:
    void signalNewText(QString text);
public slots:

};

#endif // MYTEXTEDIT_H

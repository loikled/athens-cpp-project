#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTextEdit* chatView_;
    QTextEdit* chatBox_;
    QPushButton* sendBtn_;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();

signals:
    void signalNewText(QString newText);

 public slots:
    void slotAddText(QString text);
    void slotBtnSend();
};

#endif // MAINWINDOW_H

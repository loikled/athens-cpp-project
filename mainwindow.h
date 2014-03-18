#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTcpSocket>

#include "mytextedit.h"

class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTextEdit* chatView_;
    MyTextEdit* chatBox_;
    QPushButton* sendBtn_;
    QTcpSocket sock_;
    QString host_;
    int port_;
    QString nickname_;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *e);
private:
    void init();
    void connectToServer();
    void disconnectFromServer();
    void initMenu();

signals:
    void signalNewText(const QString& newText);

 public slots:
    void slotIncomingText();
    void slotAddText(const QString& text);
    void slotBtnSend();
    void slotAbout();
    void slotSettings();
    void slotConnected();
    void slotSend(const QString& text);
};

#endif // MAINWINDOW_H

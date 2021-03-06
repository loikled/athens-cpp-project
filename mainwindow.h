#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QStringList>

#include "mytextedit.h"
#include "protocolhandler.h"

class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTextEdit* chatView_;
    MyTextEdit* chatBox_;
    QPushButton* sendBtn_;
    ProtocolHandler protocolhandler_;
    QString host_;
    QString port_;
    bool connected_;
    QStringList buffer_;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *e);
    void connectDefaults();

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
    void slotContact();
};

#endif // MAINWINDOW_H

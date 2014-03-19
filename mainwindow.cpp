#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QPushButton>
#include <QTextStream>
#include <QCoreApplication>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      chatView_(0),
      chatBox_(0),
      sendBtn_(0),
      host_("192.168.10.118"),
      port_("12345")
{
    init();
}

void MainWindow::init()
{
    QCoreApplication::setApplicationName("Project Chat Athens");
    QCoreApplication::setApplicationVersion("1.0");

    QWidget* centralw= new QWidget();

    QVBoxLayout* vlayout = new QVBoxLayout();

    chatView_ = new QTextEdit(this);   
    vlayout->addWidget(chatView_);

    chatView_->setReadOnly(true);

    QWidget *textzone = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout;
    chatBox_ = new MyTextEdit();
    hlayout->addWidget(chatBox_);
    sendBtn_ = new QPushButton("Send",this);
    hlayout->addWidget(sendBtn_,Qt::AlignRight);
    textzone->setLayout(hlayout);
    textzone->setFixedHeight(150);
    chatView_->setText("Welcome to the chat of the future");

    vlayout->addWidget(textzone);
    centralw->setLayout(vlayout);
    setCentralWidget(centralw);
    connect(sendBtn_,SIGNAL(clicked()),this,SLOT(slotBtnSend()));
    connect(chatBox_,SIGNAL(signalNewText(const QString&)),this,SLOT(slotAddText(const QString&)));
    connect(chatBox_,SIGNAL(signalNewText(const QString&)),this,SLOT(slotSend(const QString&)));
    connect(this,SIGNAL(signalNewText(const QString&)),this,SLOT(slotAddText(const QString&)));
    connect(this, SIGNAL(signalNewText(const QString&)),this,SLOT(slotSend(const QString&)));

    connect(&protocolhandler_,SIGNAL(connected()),this,SLOT(slotConnected()));

    sendBtn_->setFixedSize(70,50);
    initMenu();
}

void MainWindow::initMenu()
{
    QMenu* menu = menuBar()->addMenu("&Settings");
    QAction* actionSettings = new QAction("Server settings", this);

    menu->addAction(actionSettings);

    menu = menuBar()->addMenu("&Help");
    QAction* actionAbout = new QAction(QString("About ")+QCoreApplication::applicationName(),this);
    menu->addAction(actionAbout);

    connect(actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotAddText(const QString& text)
{
    if (chatView_)
    {
        chatView_->append(text);
    }
}

void MainWindow::slotSend(const QString& text)
{
    protocolhandler_.sendMessage(text);
}

void MainWindow::slotBtnSend()
{
    QString text= chatBox_->toPlainText();
    chatBox_->clear();
    emit signalNewText(text);
}

void MainWindow::connectToServer()
{
    slotAddText("Connecting to server...");
    protocolhandler_.connectToServer();
    slotAddText("Connected");
}

void MainWindow::disconnectFromServer()
{
    slotAddText("Disconecting...");
    protocolhandler_.disconnectFromServer();
    slotAddText("Disconnected");
}

void MainWindow::slotIncomingText()
{
   QString line = protocolhandler_.getTextMsg();
   slotAddText(line);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        disconnectFromServer();
        close();
    }
}

void MainWindow::slotSettings()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter server address"),
                                              tr("Server Address:"), QLineEdit::Normal,
                                              host_, &ok);
    if (ok && !text.isEmpty())
    {
        host_ = text;
    }
    ok = false;
    text.clear();
    text = QInputDialog::getText(this, tr("Enter server port"),
                                              tr("Port:"), QLineEdit::Normal,
                                              port_, &ok);
    if (ok && !text.isEmpty())
    {
        port_ = text;
    }
    protocolhandler_.setHostAndPort(host_, port_);
    disconnectFromServer();
    connectToServer();
}

void MainWindow::slotAbout()
{
    QMessageBox about(this);
    QString s;
    about.setText(QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion()
                   + " Author: Loik Le Devehat");
    QTextStream ss(&s);
    ss<<"This application is a graphical user interface for the athens week c++ course project";
    about.setTextFormat(Qt::RichText);
    about.setInformativeText(s);
    about.setIcon(QMessageBox::Information);
    about.adjustSize();
    about.exec();
}

void MainWindow::slotConnected()
{
    slotAddText("Connection successfull");
}

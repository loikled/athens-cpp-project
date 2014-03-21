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
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      chatView_(0),
      chatBox_(0),
      sendBtn_(0),
      host_("192.168.10.118"),
      port_("12345"),
      connected_(false)
{
    init();
}

void MainWindow::init()
{
    QCoreApplication::setApplicationName("Project Chat Athens");
    QCoreApplication::setApplicationVersion("1.0");
    setWindowIcon(QIcon("icon.png"));

    QWidget* centralw= new QWidget();

    QVBoxLayout* vlayout = new QVBoxLayout();

    chatView_ = new QTextEdit(this);   
    vlayout->addWidget(chatView_);

    chatView_->setReadOnly(true);
    chatView_->setStyleSheet("background: url(background.jpg) no-repeat bottom;");

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
    connect(chatBox_,SIGNAL(signalNewText(const QString&)),this,SLOT(slotSend(const QString&)));
    connect(this,SIGNAL(signalNewText(const QString&)),this,SLOT(slotAddText(const QString&)));
    connect(this, SIGNAL(signalNewText(const QString&)),this,SLOT(slotSend(const QString&)));

    connect(&protocolhandler_,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(&protocolhandler_, SIGNAL(NewMessage(QString)),this,SLOT(slotAddText(QString)));

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
        if (text.contains(":"))
        {
            chatView_->append("");
            QStringList list = text.split(":");
            QString name = list[0];
            list.pop_front();
            QString rest = list.join("");
            chatView_->moveCursor( QTextCursor::End );
            chatView_->moveCursor(QTextCursor::Down);
            chatView_->moveCursor(QTextCursor::StartOfLine);
            QTextCursor cursor( chatView_->textCursor() );

            QTextCharFormat formatName;
            formatName.setFontWeight( QFont::DemiBold );
            formatName.setForeground( QBrush( QColor( "green" ) ) );

            QTextCharFormat formatmsg;
            formatmsg.setFontWeight(QFont::Normal);
            formatmsg.setForeground(QBrush(QColor("black")));

            cursor.setCharFormat( formatName );
            cursor.insertText( name );
            cursor.setCharFormat(formatmsg);
            cursor.insertText(rest);
        }
        else
            chatView_->append(text);
    }
}

void MainWindow::slotSend(const QString& text)
{
    if(text.size()>0 && text != "\n")
        buffer_.append(text);
    if(connected_ && buffer_.size() > 0)
    {
        foreach(QString string, buffer_)
        {
            protocolhandler_.sendMessage(string);
        }
        buffer_.clear();
    }
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
}

void MainWindow::connectDefaults()
{
    protocolhandler_.setHostAndPort(host_,port_);
    connectToServer();
    emit signalNewText("/nick loki");
    emit signalNewText("/join #public");
}

void MainWindow::disconnectFromServer()
{
    slotAddText("Disconecting...");
    protocolhandler_.disconnectFromServer();
    slotAddText("Disconnected");
    connected_ = false;
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

        ok = false;
        text.clear();
        text = QInputDialog::getText(this, tr("Enter server port"),
                                                  tr("Port:"), QLineEdit::Normal,
                                                  port_, &ok);
        if (ok && !text.isEmpty())
        {
            port_ = text;
            disconnectFromServer();
            connectDefaults();
        }
    }
}

void MainWindow::slotAbout()
{
    QMessageBox about(this);
    QString s;
    about.setText(QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion()
                   + " Author: Loik Le Devehat");
    QTextStream ss(&s);
    ss<<"This application is a graphical user interface for the athens week c++ course project\n";
    ss<<"Gui made with Qt 5.2";
    about.setTextFormat(Qt::RichText);
    about.setInformativeText(s);
    about.setIcon(QMessageBox::Information);
    about.adjustSize();
    about.exec();
}

void MainWindow::slotConnected()
{
    slotAddText("Connection successfull");
    connected_ = true;

    //send what has been kept when not connected
    int size = buffer_.size();
    for (int i=0; i<size; i++)
        slotSend(""); // flush the buffer
}

#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      chatView_(0),
      chatBox_(0),
      sendBtn_(0)
{
    init();
}


void MainWindow::init()
{
    QSplitter* split = new QSplitter();
    chatView_ = new QTextEdit(this);
    split->insertWidget(0,chatView_);
    split->setOrientation(Qt::Vertical);
    chatView_->setReadOnly(true);

    QWidget *textzone = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout;
    chatBox_ = new QTextEdit();
    hlayout->addWidget(chatBox_);
    chatBox_->setFixedHeight(100);
    sendBtn_ = new QPushButton("Send",this);
    hlayout->addWidget(sendBtn_,Qt::AlignRight);

    textzone->setLayout(hlayout);
    split->insertWidget(2,textzone);
    chatView_->setText("Welcome to the chat of the future");
    setCentralWidget(split);
    connect(sendBtn_,SIGNAL(clicked()),this,SLOT(slotBtnSend()));
    connect(this,SIGNAL(signalNewText(QString)),this,SLOT(slotAddText(QString)));
    sendBtn_->setFixedSize(60,30);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotAddText(QString text)
{
    if (chatView_)
        chatView_->append(text);
}

void MainWindow::slotBtnSend()
{
    QString text = chatBox_->toPlainText();
    chatBox_->clear();
    emit signalNewText(text);
}

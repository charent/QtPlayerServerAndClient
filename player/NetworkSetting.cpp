#include "NetworkSetting.h"

NetworkSetting::NetworkSetting(QString &ip, int &port)
    :ip(ip),port(port)
{
    myInit();
    setWindowTitle("设置服务器参数");
}

void NetworkSetting::myInit()
{
    ipLayout = new QHBoxLayout;
    portLayout = new QHBoxLayout;
    buttonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;

    ipLabel = new QLabel("IP地址:");
    ipEdit  = new QLineEdit;
    ipEdit->setText(ip);
    ipLayout->addWidget(ipLabel,1);
    ipLayout->addWidget(ipEdit,5);
    //ipLayout->add

    portLabel = new QLabel("端口号:");
    portEdit = new QLineEdit;
    portEdit->setText(QString::number(port));
    portLayout->addWidget(portLabel,1);
    portLayout->addWidget(portEdit,5);

    confirmButton = new QPushButton("连接到服务器");
    cancelButton = new QPushButton("取消");
     buttonLayout->addStretch(1);
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(cancelButton);
     buttonLayout->addStretch(1);

    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(buttonLayout);

    confirmButton->setDisabled(true);

    connect(ipEdit,SIGNAL(textChanged(QString)),this,SLOT(ipTextChanged()));
    connect(portEdit,SIGNAL(textChanged(QString)),this,SLOT(portTextChanged()));
    connect(confirmButton,SIGNAL(clicked(bool)),this,SLOT(confirmButtonClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelButtonClicked()));

    setLayout(mainLayout);

    setMinimumSize(400,250);
}

void NetworkSetting::ipTextChanged()
{
    if (portEdit->text() == "")
    {
        return;
    }

    confirmButton->setDisabled(false);

}

void NetworkSetting::portTextChanged()
{
    if (ipEdit->text() == "")
    {
        return;
    }

    confirmButton->setDisabled(false);

}


void NetworkSetting::confirmButtonClicked()
{
    this->ip = ipEdit->text();
    this->port = portEdit->text().toInt();
    emit inputFinush();
    NetworkSetting::close();
}

void NetworkSetting::cancelButtonClicked()
{
    NetworkSetting::close();
}

NetworkSetting::~NetworkSetting()
{
    delete ipLabel;
    delete ipEdit;
    delete portLabel;
    delete portEdit;
    delete confirmButton;
    delete cancelButton;
    delete ipLayout;
    delete portLayout;
    delete buttonLayout;
    delete mainLayout;
}

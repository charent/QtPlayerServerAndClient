#ifndef NETWORKSETTING_H
#define NETWORKSETTING_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class NetworkSetting : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkSetting(QString &ip,int &port);
    ~NetworkSetting();
    void myInit();
signals:
    void inputFinush();
public slots:
    void confirmButtonClicked();
    void cancelButtonClicked();
    void ipTextChanged();
    void portTextChanged();
private:
    QLabel *ipLabel;
    QLineEdit *ipEdit;
    QLabel *portLabel;
    QLineEdit *portEdit;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *ipLayout;
    QHBoxLayout *portLayout;
    QHBoxLayout *buttonLayout;

    QString &ip;
    int &port;
};

#endif // NETWORKSETTING_H

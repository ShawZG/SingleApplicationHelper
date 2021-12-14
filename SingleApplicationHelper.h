#ifndef SINGLEAPPLICATIONHELPER_H
#define SINGLEAPPLICATIONHELPER_H

#include <QObject>
#include <QLocalServer>

class SingleApplicationHelper : public QObject
{
    Q_OBJECT
public:
    enum Mode {User = 1 << 0, System = 1 << 1};
    Q_DECLARE_FLAGS(Options, Mode)

    bool registerApplication(const QString &appName, Options options = Mode::User, int timeout = 300);
    bool sendSecondApplicationArguments(const QString &appName, const QString &arguments, int timeout = 300);

private slots:
    void slotNewLocalConnect();

signals:
    void sigSecondApplicationArguments(QString arguments);

private:
    QLocalServer localServer;
};

#endif // SINGLEAPPLICATIONHELPER_H

#include "SingleApplicationHelper.h"

#include <QLocalSocket>
#include <QDataStream>
#include <QFile>
#include <QDir>

bool SingleApplicationHelper::registerApplication(const QString &appName, Options options, int timeout)
{
    QLocalSocket socket;
    socket.connectToServer(appName);
    if (socket.waitForConnected(timeout)) {
        return false;
    }

    if( options & Mode::User ){
        localServer.setSocketOptions(QLocalServer::UserAccessOption );
    } else {
        localServer.setSocketOptions(QLocalServer::WorldAccessOption );
    }

    connect(&localServer, &QLocalServer::newConnection, this, &SingleApplicationHelper::slotNewLocalConnect);

    // Note: On Unix if the server crashes without closing listen will fail with AddressInUseError.
    // To create a new server the file should be removed.
    // On Windows two local servers can listen to the same pipe at the same time, but any connections will go to one of the server.
    if (!localServer.listen(appName)
        && QAbstractSocket::AddressInUseError == localServer.serverError()) {
        // Read Qt sources file, we know the path.
        QString socketFile = QDir::tempPath() + '/' + localServer.serverName();
        QFile::remove(socketFile);
        localServer.listen(appName);
    }
    return true;
}

bool SingleApplicationHelper::sendSecondApplicationArguments(const QString &appName, const QString &arguments, int timeout)
{
    QLocalSocket socket;
    socket.connectToServer(appName);
    if (!socket.waitForConnected(timeout)) {
        return false;
    }
    QDataStream stream(&socket);
    stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    stream << arguments;
    socket.waitForBytesWritten();
    return true;
}

void SingleApplicationHelper::slotNewLocalConnect()
{
    QLocalSocket *socket = localServer.nextPendingConnection();
    if (nullptr != socket) {
        socket->waitForReadyRead(500);
        QDataStream stream(socket);
        stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        QString args;
        stream >> args;
        emit sigSecondApplicationArguments(args);
    }
}

#include "server.h"


server::server(const QHostAddress &host, quint16 port, QObject *parent) : QObject(parent)
{
       m_TcpServer = new QTcpServer(this);

       if (!m_TcpServer->listen(host, port))
       {
           log(QString("Server Error! %1").arg(m_TcpServer->errorString()));
           m_TcpServer->close();
           return;
       }

       connect(m_TcpServer, SIGNAL(newConnection()),
               this, SLOT(slotNewConnection()));

}

void server::log(QString logs)
{
     QFile file(QDir::currentPath()+"/log.txt");
     if(file.open(QIODevice::Append | QIODevice::Text)) {
                    QTextStream writeStream(&file);
                    writeStream << QDateTime::currentDateTime().toString() << logs << '\n';
                    file.close();
                }

}

void server::sendToClient(QTcpSocket *sock, QString file)
{
    QByteArray arr;
    QDataStream out(&arr,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);


    out << quint16(0) << QTime::currentTime()
                      << QString("Файл %1 получен.").arg(file);
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));

    sock->write(arr);

}


void server::disconnectSocket()
{
    QTcpSocket * sock = qobject_cast<QTcpSocket *>(sender());
    if (NULL == sock) {
       return;
    }

     QString messageToDisconnect = sock->peerAddress().toString() +
                                   sock->localPort() + "disconected";

     log(messageToDisconnect);
}


void server::slotRead()
{
    QDataStream in(m_TcpSocket);
    in.setVersion(QDataStream::Qt_5_11);

    for(;;) {
        if (!m_blockSize) {
            if (m_TcpSocket->bytesAvailable() < sizeof(qint64)) {
                break;
            }
            in >> m_blockSize;
        }

        if (m_TcpSocket->bytesAvailable() < m_blockSize) {
            break;
        }


        QString fileName;
//        in >> fileName;
         in >> fileName
            >> m_sizeFile;

        QByteArray line = m_TcpSocket->readAll();
        QFile target(QDir::currentPath() + "/downloads/"+fileName);

        if (!target.open(QIODevice::WriteOnly)) {
             log("Error! Can't open file for written");
            return;
        }

        target.write(line);
        target.close();

        m_blockSize = 0;

        if(target.size() == m_sizeFile)
        {
            sendToClient(m_TcpSocket,fileName);
        }
    }
}

void server::slotNewConnection()
{
       m_TcpSocket = m_TcpServer->nextPendingConnection();

       QString connectMessage = m_TcpSocket->peerAddress().toString()+
                                m_TcpSocket->localPort() + "connected";

       log(connectMessage);

       connect(m_TcpSocket, SIGNAL(readyRead()),
               this, SLOT(slotRead()));

       connect(m_TcpSocket, SIGNAL(disconnected()),
               this, SLOT(disconnectSocket()));

       connect(m_TcpSocket, SIGNAL(disconnected()),
               m_TcpSocket, SLOT(deleteLater()));

}

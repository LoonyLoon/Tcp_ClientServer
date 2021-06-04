#include "client.h"


client::client(const QString &ip, quint16 port, const QString file, QObject *parent)
: QObject(parent)
{
    m_TcpSocket = new QTcpSocket(this);
    m_TcpSocket->connectToHost(ip,port);

    m_File = new QFile(file);
    m_FileName = QFileInfo(file).fileName();
    m_sizeFile = QFileInfo(file).size();


    connect(m_TcpSocket, SIGNAL(connected()),this,SLOT(connectSuccessful()));
    connect(m_TcpSocket,SIGNAL(readyRead()),this,SLOT(answer()));
}

client::~client()
{
    delete m_TcpSocket;
    delete m_File;
}

void client::connectSuccessful()
{
   sendFile();
}

void client::sendFile()
{
    QByteArray SendDataBytes;
    QDataStream out(&SendDataBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);

    if(!m_File->open(QIODevice::ReadOnly))
    {
        qInfo()<<"Error. File not open";
        return;
    }
    else {
//        out << (qint64)0 << m_FileName;
        out << (qint64)0 << m_FileName << m_sizeFile;

        QByteArray q = m_File->readAll();
        SendDataBytes.append(q);
        m_File->close();

        out.device()->seek(0);
        out << (qint64)(SendDataBytes.size() - sizeof(qint64));

        qint64 x = 0;
        while (x < SendDataBytes.size()) {
            quint64 y = m_TcpSocket->write(SendDataBytes);
            x += y;
        }
    }

}

void client::answer()
{
    QDataStream in(m_TcpSocket);
    in.setVersion(QDataStream::Qt_5_11);

    for(;;)
    {

        if(!m_blockSize) {
            if (m_TcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_blockSize;
        }

        if (m_TcpSocket->bytesAvailable() < m_blockSize) {
            break;
        }

        QTime time;
        QString message;

        in >>time>>message;

        qInfo() << time.toString() << message;
        m_blockSize = 0;
    }



}


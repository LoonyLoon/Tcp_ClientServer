#ifndef CLIENT_H
#define CLIENT_H

#include <QFile>
#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QDebug>
#include <QTime>


class client : public QObject
{
     Q_OBJECT

public:
    explicit client(const QString &ip, quint16 port, const QString file, QObject *parent = nullptr);
    ~client();


private slots:
    void connectSuccessful();   //If the connection is established then start the file transfer
    void sendFile();            // Send file
    void answer();              //Server response that the file has been delivered

private:
    QTcpSocket *m_TcpSocket;
    QFile      *m_File;
    QString     m_FileName;

    qint64      m_sizeFile;
    quint16     m_blockSize;
};

#endif // CLIENT_H

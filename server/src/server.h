#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QDir>


class server : public QObject
{

    Q_OBJECT

public:
    explicit server(const QHostAddress &host, quint16 port, QObject *parent = nullptr);

private slots:
    void slotRead();            //read data
    void slotNewConnection();   //check connection clients
    void disconnectSocket();    //check disconect clients


private:
    QTcpServer  *m_TcpServer;
    QTcpSocket  *m_TcpSocket;
    qint64       m_blockSize = 0; //size pack
    qint64       m_sizeFile;


    void log(QString logs);       //method for writes log
    void sendToClient(QTcpSocket *sock, QString file);

};

#endif // SERVER_H

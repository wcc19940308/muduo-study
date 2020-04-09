#include "Acceptor.h"
#include "TcpStream.h"

#include <stdio.h>
#include <sys/socket.h>

// 封装accept操作，构造函数中初始化socket, bind, listen
Acceptor::Acceptor(const InetAddress& listenAddr)
    : listenSock_(Socket::createTCP())
{
    listenSock_.setReuseAddr(true);
    listenSock_.bindOrDie(listenAddr);
    listenSock_.listenOrDie();
}

// RAII管理accept的链接
TcpStreamPtr Acceptor::accept()
{
    int sockfd = ::accept(listenSock_.fd(), NULL, NULL);
    if (sockfd >= 0)
    {
        return TcpStreamPtr(new TcpStream(Socket(sockfd)));
    }
    else
    {
        perror("Acceptor::accept");
        return TcpStreamPtr();
    }
}
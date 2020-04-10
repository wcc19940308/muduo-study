#include "main/Acceptor.h"
#include "main/InetAddress.h"
#include "main/TcpStream.h"

#include <thread>
#include <string.h>

// a thread-per-connection current echo server
int main(int argc, char* argv[])
{
    // 初始化struct sockaddr
    InetAddress listenAddr(3007);
    // socket, bind, listen初始化工作
    Acceptor acceptor(listenAddr);
    printf("Accepting... Ctrl-C to exit\n");
    int count = 0;
    bool nodelay = argc > 1 && strcmp(argv[1], "-D") == 0;
    while (true)
    {
        TcpStreamPtr tcpStream = acceptor.accept(); // RAII管理每个连接
        printf("accepted no. %d client\n", ++count);
        if (nodelay)
            tcpStream->setTcpNoDelay(true);
        std::thread thr([count] (TcpStreamPtr stream) {
            printf("thread for no. %d client started.\n", count);

            // uint32_t dataLength = 0; 加上length长度
            // stream->receiveAll(&dataLength, sizeof(dataLength));
            // dataLength = ntohl(dataLength);
            // printf("dataLength is %d\n", dataLength);
            // char buf[dataLength]; 
            char buf[4096];
            int nr = 0;
            // 服务端把收到的都一次行发出去，读4KB 
            while ((nr = stream->receiveSome(buf, sizeof(buf))) > 0)
            {
                int nw = stream->sendAll(buf, nr);
                if (nw < nr)
                {
                    break;
                }
            }
            printf("thread for no. %d client ended.\n", count);
        }, std::move(tcpStream));
        thr.detach();
    }
}
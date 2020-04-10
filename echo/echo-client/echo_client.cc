#include "main/InetAddress.h"
#include "main/TcpStream.h"
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    assert(argc >= 3);
    const int len = atoi(argv[2]);
    InetAddress addr(3007);
    // 解析地址，获得sockaddr
    if (!InetAddress::resolve(argv[1], &addr))
    {
        printf("Unable to resolve %s\n", argv[1]);
        return 0;
    }

    printf("connecting to %s\n", addr.toIpPort().c_str());
    TcpStreamPtr stream(TcpStream::connect(addr));
    if (!stream)
    {
        printf("Unable to connect %s\n", addr.toIpPort().c_str());
        perror("");
        return 0;
    }    
    printf("connected, sending %d bytes\n", len);

    // // 发送数据长度的消息
    // int32_t length = htonl(len);
    // stream->sendAll(&length, sizeof(length));
    // printf("send length = %d message\n", len);

    std::string message(len, 'S');
    int nw = stream->sendAll(message.c_str(), message.size());
    printf("send %d bytes\n", nw);

    if (argc > 3)
    {
        for (char cmd : std::string(argv[3]))
        {
            if (cmd == 's')  // shutdown
            {
                printf("shutdown write\n");
                stream->shutdownWrite();
            }
            else if (cmd == 'p') // pause
            {
                printf("sleeping for 10 seconds\n");
                ::sleep(10);
                printf("done\n");
            }
            else if (cmd == 'c') // close
            {
                printf("close without reading response\n");
                return 0;
            }
            else
            {
                printf("unknown command '%c'\n", cmd);
            }
        }
    }
    // 客户端每次一次性发完，然后一次性收完
    std::vector<char> receive(len);
    int nr = stream->receiveAll(receive.data(), receive.size());
    printf("received %d bytes\n", nr);
    if (nr != nw)
    {
        printf("!!! Incomplete response !!!\n");
    }
}   
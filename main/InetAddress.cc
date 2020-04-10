#include "InetAddress.h"

#include <assert.h>
#include <netdb.h>
#include <string.h>
#include <strings.h> // bzero
#include <arpa/inet.h>

InetAddress::InetAddress(uint16_t port, bool loopbackOnly)
{
  ::bzero(&saddr_, sizeof(saddr_));
  saddr_.sin_family = AF_INET;
  saddr_.sin_addr.s_addr = htonl(loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY);
  saddr_.sin_port = htons(port);
}

// 返回一个string的拷贝，effective c++中明确尽量返回值
std::string InetAddress::toIp() const
{
    char buf[32] = "";
    ::inet_ntop(AF_INET, &saddr_.sin_addr, buf, sizeof(buf));
    return buf;
}

std::string InetAddress::toIpPort() const
{
    char buf[32] = ""; // 
    ::inet_ntop(AF_INET, &saddr_.sin_addr, buf, sizeof(buf));
    int end = ::strlen(buf);
    uint16_t port = portHostEndian();
    snprintf(buf + end, sizeof buf - end, ":%u", port);
    return buf;
}

static const int kResolveBufSize = 4096;
bool InetAddress::resolveSlow(const char* hostname, InetAddress* out)
{
    std::vector<char> buf(2 * kResolveBufSize);
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    bzero(&hent, sizeof(hent));

    while (buf.size() <= 16 * kResolveBufSize) // 64kb
    {
        // he存储成功返回的指向hent的指针
        int ret = gethostbyname_r(hostname, &hent, buf.data(), buf.size(), &he, &herrno);
        if (ret == 0 && he != NULL)
        {
            assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
            out->saddr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
            return true;
        }
        // 数据太大
        else if (ret == ERANGE)
        {
            buf.resize(buf.size() * 2);
        }
        else
        {
            if (ret)
             perror("InetAddress::reslove");
            return false;
        }
    }
    return false;
}

// 做通用化处理，解析hostname的ip地址并赋值给out
bool InetAddress::resolve(StringArg hostname, InetAddress* out)
{
    assert(out);
    char buf[kResolveBufSize];
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    bzero(&hent, sizeof(hent));

    int ret = gethostbyname_r(hostname.c_str(), &hent, buf, sizeof buf, &he, &herrno);
    if (ret == 0 && he != NULL)
    {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->saddr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else if (ret == ERANGE)
    {
        return resolveSlow(hostname.c_str(), out);
    }
    else
    {
        if (ret)
         perror("InetAddress::resolve");
        return false;
    }
}
#pragma once

#include <cstdint>
#include <cassert>
#include <string>
#include <string_view>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#undef GetAddrInfo

namespace socket_helper {

    using PORT = uint16_t;

    struct SockInfo {
        enum class Family {
            IPv4,
            IPv6,
        };
        enum class Type {
            TCP,
            UDP
        };

        Family family;
        Type   type;
    };

    SOCKET Create(int family, int type, int protocol = 0);
    SOCKET Create(const SockInfo& sock_info = { SockInfo::Family::IPv4, SockInfo::Type::TCP });

    void Close(SOCKET* sock);

    void SetNonBlocking(SOCKET* sock);
    void SetBlocking(SOCKET* sock);

    bool GetAddrInfo(std::string_view host, PORT port, PADDRINFOA addr_info);

    void Bind(SOCKET sock, const SOCKADDR& sock_addr);
    
    void Listen(SOCKET sock, int backlog);
    
    SOCKET Accept(SOCKET sock, PSOCKADDR sock_addr);
    
    bool Connect(SOCKET sock, const SOCKADDR& sock_addr, int time_out_ms = 0);
    
    int Send(SOCKET sock, std::string_view data);
    int Send(SOCKET sock, std::string_view data, const SOCKADDR& sock_addr);
    
    std::string Recv(SOCKET sock);
    std::string Recv(SOCKET sock, PSOCKADDR sock_addr);

    std::string GetIPAddr(const ADDRINFOA& addr_info);
    std::string CheckRecvData(char* buf, int recv_byte);

}

/*
htonl 	    ホスト・バイトオーダーをネットワーク・バイトオーダーに変換 (4バイト)
htons 	    ホスト・バイトオーダーをネットワーク・バイトオーダーに変換 (2バイト)
ntohl 	    ネットワーク・バイトオーダーをホスト・バイトオーダーに変換 (4バイト)
ntohs 	    ネットワーク・バイトオーダーをホスト・バイトオーダーに変換 (2バイト)
inet_addr 	文字列をIPアドレスに変換
inet_ntoa 	IPアドレスを文字列に変換
*/

/*
ADDRINFOA;
SOCKADDR;

SOCKADDR_IN;
IN_ADDR;
*/

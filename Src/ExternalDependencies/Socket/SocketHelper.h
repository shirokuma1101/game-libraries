#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_SOCKET_SOCKETHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_SOCKET_SOCKETHELPER_H_

#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>
#ifdef _WINSOCKAPI_
#error Please include SocketHelper.h before winsock.h (Maybe in Windows.h)
#endif // _WINSOCKAPI_
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

#include "Math/Convert.h"
#include "Utility/Assert.h"
#include "Utility/Macro.h"

#include "strconv.h"

#undef GetAddrInfo

namespace socket_helper {

    using PORT           = uint16_t;
    constexpr int BUFFER = 4096;
    constexpr int IPv4   = AF_INET;
    constexpr int IPv6   = AF_INET6;
    constexpr int TCP    = SOCK_STREAM;
    constexpr int UDP    = SOCK_DGRAM;

    NAMESPACE_EXTERNAL_BEGIN
    
    NAMESPACE_INTERNAL_BEGIN
    inline std::string GetWSAErrorDetail() {
        LPVOID msg_buf = nullptr;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
            WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&msg_buf, 0, NULL
        );
        std::string error_detail = wide_to_sjis((LPCTSTR)msg_buf);
        LocalFree(msg_buf);
        return error_detail;
    }
    inline std::string MakeErrorDetails(std::string_view detail, int err) {
        return std::string(detail) + "Error code: " + std::to_string(err) + "(" + std::to_string(WSAGetLastError()) + ")\n" + GetWSAErrorDetail();
    }
    inline std::string CheckRecvData(char* buf, int recv_byte) {
        if (recv_byte) {
            if (recv_byte > BUFFER) {
                assert::RaiseAssert("Buffer overflow.");
            }
            buf[recv_byte] = '\0';
            return std::string(buf, recv_byte);
        }
        return std::string();
    }
    NAMESPACE_INTERNAL_END
    
    inline SOCKET Create(int family = IPv4, int type = TCP, int protocol = 0) {
        WSADATA wsa_data{};
        SecureZeroMemory(&wsa_data, sizeof(wsa_data));

        if (FAIL_CHECK(WSAStartup(WINSOCK_VERSION, &wsa_data), err)) {
            switch (err) {
            case WSAEFAULT:
            case WSAEINPROGRESS:
            case WSAEPROCLIM:
            case WSASYSNOTREADY:
            case WSAVERNOTSUPPORTED:
                assert::ShowWarning("https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup");
                assert::RaiseAssert(detail::MakeErrorDetails("WSAStartup failed.", err));
                break;
            default:
                assert::RaiseAssert(detail::MakeErrorDetails("Unknown error.", err));
                break;
            }
            return SOCKET();
        }

        if (!protocol) {
            if (type == TCP) {
                protocol = IPPROTO_TCP;
            }
            else if (type == UDP) {
                protocol = IPPROTO_UDP;
            }
        }

        return socket(family, type, protocol);
    }
    
    inline void Close(SOCKET* sock, bool wsa_cleanup = true) {
        if (FAIL_CHECK(closesocket(*sock), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("closesocket failed.", err));
            return;
        }
        *sock = SOCKET();
        if (!wsa_cleanup) return;
        if (FAIL_CHECK(WSACleanup(), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("WSACleanup failed.", err));
        }
    }

    inline void SetNonBlocking(SOCKET* sock) {
        u_long mode = 1;
        if (FAIL_CHECK(ioctlsocket(*sock, FIONBIO, &mode), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("non-blocking mode failed.", err));
        }
    }
    
    inline void SetBlocking(SOCKET* sock) {
        u_long mode = 0;
        if (FAIL_CHECK(ioctlsocket(*sock, FIONBIO, &mode), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("blocking mode failed.", err));
        }
    }

    inline void Bind(SOCKET* sock, const ADDRINFO& addr_info) {
        if (FAIL_CHECK(bind(*sock, addr_info.ai_addr, convert::SizeOf<int>(*addr_info.ai_addr)), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("bind failed.", err));
        }
    }
    
    inline void Listen(SOCKET* sock, int backlog) {
        if (FAIL_CHECK(listen(*sock, backlog), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("listen failed.", err));
        }
    }
    
    inline SOCKET Accept(SOCKET* sock, ADDRINFO* addr_info) {
        SecureZeroMemory(addr_info, sizeof(*addr_info));
        int size = convert::SizeOf<int>(*addr_info->ai_addr);
        return accept(*sock, addr_info->ai_addr, &size);
    }

    inline bool Connect(SOCKET* sock, const ADDRINFO& addr_info, int time_out_ms = 0) {
        if (time_out_ms) {
            SetNonBlocking(sock);
            if (FAIL_CHECK(connect(*sock, addr_info.ai_addr, convert::SizeOf<int>(*addr_info.ai_addr)), err)) {
                if (err == SOCKET_ERROR) {
                    err = WSAGetLastError();
                    SetBlocking(sock);
                    if (err != WSAEWOULDBLOCK) {
                        assert::RaiseAssert(detail::MakeErrorDetails("Unexpected error occurred.", err));
                        return false;
                    }
                }
                else {
                    assert::RaiseAssert(detail::MakeErrorDetails("Unexpected socket error occurred.", err));
                    return false;
                }
            }

            fd_set readfds{}, writefds{}, exceptfds{};
            timeval timeout{};
            FD_ZERO(&readfds);
            FD_ZERO(&writefds);
            FD_ZERO(&exceptfds);
            FD_SET(*sock, &readfds);
            FD_SET(*sock, &writefds);
            FD_SET(*sock, &exceptfds);
            SecureZeroMemory(&timeout, sizeof(timeout));
            timeout.tv_usec = convert::MSToUS(time_out_ms);

            // if return 0 timeout
            if (SUCCESS_CHECK(select(convert::SizeOf<int>(*sock + 1), &readfds, &writefds, &exceptfds, &timeout), err)) {
                assert::ShowWarning("Timeout: " + std::string(addr_info.ai_canonname));
                return false;
            }
            if (FD_ISSET(*sock, &readfds) || FD_ISSET(*sock, &writefds)) {
                return true;
            }
        }
        else {
            if (FAIL_CHECK(connect(*sock, addr_info.ai_addr, convert::SizeOf<int>(*addr_info.ai_addr)), err)) {
                assert::RaiseAssert(detail::MakeErrorDetails("Cannot connect.", err));
                return false;
            }
            else {
                return true;
            }
        }
        return false;
    }
    
    inline int Send(SOCKET sock, std::string_view data) {
        return send(sock, data.data(), static_cast<int>(data.size()), 0);
    }
    
    inline int Send(SOCKET sock, std::string_view data, const SOCKADDR& sock_addr) {
        return sendto(sock, data.data(), static_cast<int>(data.size()), 0, &sock_addr, sizeof(sock_addr));
    }
    
    inline std::string Recv(SOCKET sock) {
        char buf[BUFFER];
        return detail::CheckRecvData(buf, recv(sock, buf, BUFFER, 0));
    }
    
    inline std::string Recv(SOCKET sock, ADDRINFO* addr_info) {
        char buf[BUFFER];
        int size = convert::SizeOf<int>(*addr_info->ai_addr);
        return detail::CheckRecvData(buf, recvfrom(sock, buf, BUFFER, 0, addr_info->ai_addr, &size));
    }

    inline bool GetAddrInfo(std::string_view host, PORT port, ADDRINFO* addr_info) {
        SecureZeroMemory(addr_info, sizeof(*addr_info));

        ADDRINFO* result = nullptr, * next = nullptr;
        ADDRINFO hints{};
        SecureZeroMemory(&hints, sizeof(hints));
        hints.ai_flags    = AI_CANONNAME;
        hints.ai_family   = AF_INET;     // IPv4限定 AF_UNSPEC:全て受け入れる
        hints.ai_socktype = SOCK_STREAM; // TCPで送信
        hints.ai_protocol = IPPROTO_TCP; // 受け取りをTCPに限定

        if (FAIL_CHECK(getaddrinfo(host.data(), std::to_string(port).c_str(), &hints, &result), err)) {
            assert::RaiseAssert(detail::MakeErrorDetails("Domain not found.", err));
            return false;
        }

        if (!result) {
            assert::RaiseAssert("Domain not found.");
            return false;
        }
        
        *addr_info = *result;
        for (next = result; next != NULL; next = next->ai_next) {
            SOCKET sock = Create(next->ai_family, next->ai_socktype, next->ai_protocol);
            if (Connect(&sock, *next, 1000)) {
                Close(&sock);
                *addr_info = *result;
                continue;
            }
            Close(&sock);
        }
        //freeaddrinfo(result);

        return true;
    }

    inline std::string GetIPAddr(const ADDRINFO& addr_info) {
        if (!addr_info.ai_addr) return std::string();
        
        SOCKADDR_IN* sock_addr_in = reinterpret_cast<SOCKADDR_IN*>(addr_info.ai_addr);
        char dst[32];

        inet_ntop(addr_info.ai_family, &sock_addr_in->sin_addr, dst, sizeof(dst));
        return std::string(dst);
    }
    
    NAMESPACE_EXTERNAL_END
}

#endif

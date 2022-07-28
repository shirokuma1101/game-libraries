#include "SocketHelper.h"

SOCKET socket_helper::Create(int family, int type, int protocol)
{
    WSADATA wsa_data{};

    if (SOCKET_HELPER_CHECK_ERROR(WSAStartup(WINSOCK_VERSION, &wsa_data), err)) {
        switch (err) {
            // note "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup"
        case WSAEFAULT:
        case WSAEINPROGRESS:
        case WSAEPROCLIM:
        case WSASYSNOTREADY:
        case WSAVERNOTSUPPORTED:
        default:
            break;
        }
    }

    if (!protocol) {
        if (type == SOCK_STREAM) {
            protocol = IPPROTO_TCP;
        }
        else if (type == SOCK_DGRAM) {
            protocol = IPPROTO_UDP;
        }
    }

    return socket(family, type, protocol);
}

SOCKET socket_helper::Create(const SockInfo& sock_info)
{
    int family{}, type{};

    if (sock_info.family == SockInfo::Family::IPv4) {
        family = AF_INET;
    }
    else if (sock_info.family == SockInfo::Family::IPv6) {
        family = AF_INET6;
    }

    if (sock_info.type == SockInfo::Type::TCP) {
        type = SOCK_STREAM;
    }
    else if (sock_info.type == SockInfo::Type::UDP) {
        type = SOCK_DGRAM;
    }

    return Create(family, type);
}

void socket_helper::Close(SOCKET* sock)
{
    if (SOCKET_HELPER_CHECK_ERROR(closesocket(*sock), err)) {
        assert(0);
    }
    if (SOCKET_HELPER_CHECK_ERROR(WSACleanup(), err)) {
        assert(0);
    }
    *sock = SOCKET();
}

void socket_helper::SetNonBlocking(SOCKET* sock)
{
    u_long mode = 1;
    if (SOCKET_HELPER_CHECK_ERROR(ioctlsocket(*sock, FIONBIO, &mode), err)) {
        assert(0);
    }
}

void socket_helper::SetBlocking(SOCKET* sock)
{
    u_long mode = 0;
    if (SOCKET_HELPER_CHECK_ERROR(ioctlsocket(*sock, FIONBIO, &mode), err)) {
        assert(0);
    }
}

bool socket_helper::GetAddrInfo(std::string_view host, PORT port, PADDRINFOA addr_info)
{
    SecureZeroMemory(addr_info, sizeof(*addr_info));

    ADDRINFOA hints{};
    SecureZeroMemory(&hints, sizeof(hints));
    hints.ai_flags    = AI_CANONNAME;
    hints.ai_family   = AF_INET;     // IPv4ŒÀ’è AF_UNSPEC ‘S‚ÄŽó‚¯“ü‚ê‚é
    hints.ai_socktype = SOCK_STREAM; // TCP‚Å‘—M
    hints.ai_protocol = IPPROTO_TCP; // Žó‚¯Žæ‚è‚ðTCP‚ÉŒÀ’è

    PADDRINFOA result, next;

    if (SOCKET_HELPER_CHECK_ERROR(getaddrinfo(host.data(), std::to_string(port).c_str(), &hints, &result), err)) {
        // no exits domain
        //assert(0);
    }

    if (result) {
        *addr_info = *result;
    }
    
    for (next = result; next != NULL; next = next->ai_next) {
        SOCKET sock = Create(next->ai_family, next->ai_socktype, next->ai_protocol);

        if (Connect(sock, *next->ai_addr, 200)) {
            Close(&sock);
            *addr_info = *next;
            break;
        }
    }

    freeaddrinfo(result);

    return true;
}

void socket_helper::Bind(SOCKET sock, const SOCKADDR& sock_addr)
{
    if (SOCKET_HELPER_CHECK_ERROR(bind(sock, &sock_addr, sizeof(sock_addr)), err)) {
        assert(0);
    }
}

void socket_helper::Listen(SOCKET sock, int backlog)
{
    if (SOCKET_HELPER_CHECK_ERROR(listen(sock, backlog), err)) {
        assert(0);
    }
}

SOCKET socket_helper::Accept(SOCKET sock, PSOCKADDR sock_addr)
{
    int size = sizeof(sock_addr);
    return accept(sock, sock_addr, &size);
}

bool socket_helper::Connect(SOCKET sock, const SOCKADDR& sock_addr, int time_out_ms)
{
    if (time_out_ms) {
        SetNonBlocking(&sock);

        if (SOCKET_HELPER_CHECK_ERROR(connect(sock, &sock_addr, static_cast<int>(sizeof(sock_addr))), err)) {
            if (err == SOCKET_ERROR) {
                err = WSAGetLastError();
                SetBlocking(&sock);
                if (err != WSAEWOULDBLOCK) {
                    return false;
                }
            }
            else {
                assert(0);
                return false;
            }
        }

        fd_set readfds, writefds, exceptfds;
        timeval timeout{};

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_SET(sock, &readfds);
        FD_SET(sock, &writefds);
        FD_SET(sock, &exceptfds);
        SecureZeroMemory(&timeout, sizeof(timeout));
        timeout.tv_usec = convert::MSToUS(time_out_ms);

        // if return 0 timeout
        if (SOCKET_HELPER_CHECK_NOT_ERROR(select(static_cast<int>(sock + 1), &readfds, &writefds, &exceptfds, &timeout), err)) {
            return false;
        }
        if (FD_ISSET(sock, &readfds) || FD_ISSET(sock, &writefds)) {
            return true;
        }
    }
    else {
        if (SOCKET_HELPER_CHECK_ERROR(connect(sock, &sock_addr, static_cast<int>(sizeof(sock_addr))), err)) {
            return false;
        }
        else {
            return true;
        }
    }

    return false;

}

void socket_helper::Send(SOCKET sock, std::string_view data)
{
    int send_byte = send(sock, data.data(), static_cast<int>(strlen(data.data())), 0);
}

void socket_helper::Send(SOCKET sock, std::string_view data, const SOCKADDR& sock_addr)
{
    int send_byte = sendto(sock, data.data(), static_cast<int>(strlen(data.data())), 0, &sock_addr, sizeof(sock_addr));
}

std::string socket_helper::Recv(SOCKET sock)
{
    const int buf_size = 4096;
    char buf[buf_size];

    int recv_byte = recv(sock, buf, buf_size, 0);
    return CheckRecvData(buf, recv_byte);
}

std::string socket_helper::Recv(SOCKET sock, PSOCKADDR sock_addr)
{
    const int buf_size = 4096;
    char buf[buf_size];
    int size = sizeof(sock_addr);

    int recv_byte = recvfrom(sock, buf, buf_size, 0, sock_addr, &size);
    return CheckRecvData(buf, recv_byte);
}

std::string socket_helper::GetIPAddr(const ADDRINFOA& addr_info)
{
    if (!addr_info.ai_addr) return std::string();

    PSOCKADDR_IN sock_addr_in = (PSOCKADDR_IN)addr_info.ai_addr;
    IN_ADDR      in_addr = (IN_ADDR)sock_addr_in->sin_addr;
    char dst[32];

    inet_ntop(addr_info.ai_family, &in_addr, dst, sizeof(dst));
    return std::string(dst);
}

std::string socket_helper::CheckRecvData(char* buf, int recv_byte)
{
    if (recv_byte) {
        assert(recv_byte == 4096);
        buf[recv_byte] = '\0';
        return std::string(buf);
    }
    return std::string();
}

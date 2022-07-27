#pragma once

#include <cstdint>
#include <cassert>
#include <string>
#include <string_view>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef INC_CONVERT
namespace convert {
	template<class T>          constexpr T SToNS(T t)  { return t * 1000000000; }
	template<class T>          constexpr T SToUS(T t)  { return t * 1000000; }
	template<class T>          constexpr T SToMS(T t)  { return t * 1000; }
	template<class T>          constexpr T MSToNS(T t) { return t * 1000000; }
	template<class T>          constexpr T MSToUS(T t) { return t * 1000; }
	template<class T>          constexpr T USToNS(T t) { return t * 1000; }
}
#endif // !INC_CONVERT

#define SOCKET_HELPER_CHECK_ERROR(func, err) int err = func; err
#define SOCKET_HELPER_CHECK_NOT_ERROR(func, err) int err = func; !err

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

	bool Connect(SOCKET sock, const SOCKADDR& sock_addr, int time_out_ms = 0);

	std::string GetIPAddr(const ADDRINFOA& addr_info);

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

////////////////////////

//connect(sock, res->ai_addr, res->ai_addrlen))

//////////////////////////
//
//LPSOCKADDR sa;
//TCHAR buf[48];
//DWORD buflen;

//for (ptr = ppResult; ptr != NULL; ptr = ptr->ai_next) {
//    memset(buf, 0, sizeof(buf));
//    buflen = 48;

//    switch (ptr->ai_family) {
//    case AF_INET:
//    case AF_INET6:
//        sa = (LPSOCKADDR)ptr->ai_addr;
//        result = WSAAddressToString(sa, (DWORD)ptr->ai_addrlen, NULL, buf, &buflen);
//        break;
//    }
//}
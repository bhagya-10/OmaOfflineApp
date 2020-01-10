#ifndef _NET_H
#define _NET_H

#ifdef __cplusplus
extern "C" {
#endif

///GPRS setting struct
typedef struct _net_profile{
	char apn[100];
	char user[100];
	char password[100];
}net_profile;

///Socket TCP setting struct
typedef struct _socket_profile{
	char ip[16];
	int port;
}socket_profile;

/*!
* @brief Start up GPRS,PPP network
*
* @param net_profile:the gprs setting profile
*
* @return 0 success,-1 fail
*/
int initializePPP(net_profile* pro);

/*!
* @brief Disconnect to GPRS
*
* @return 0 success,-1 fail
*/
int disconnectPPP(void);

/*!
* @brief Connect TCP/IP to a ip,port
*
* @param socket_profile:the tcp setting profile
*
* @return >0 SocketID,<0 fail
*/
int Net_Connect(socket_profile* pro);

/*!
* @brief Send data on socket
*
* @param SocketID:the socketID return by Net_Connect
* @param buf:data to send
* @param len: length of the data
*
* @return >0 byte sent,<0 fail
*/
int Net_Send(int SocketID,char * buf,int len);

/*!
* @brief Receive data on socket
*
* @param SocketID:the socketID return by Net_Connect
* @param buf: buf of receive
* @param len: Max length of receive buf
* @param timeout: timeout value, 1s = 1000
*
* @return >0 byte receive,<0 fail
*/
int Net_Receive(int SocketID,char * buf,int len,int timeout);

/*!
* @brief Close the TCP socket
*
* @param SocketID:the socketID return by Net_Connect
*
* @return >0 success,<0 fail
*/
int Net_Close(int SocketID);

#ifdef __cplusplus
}
#endif

#endif

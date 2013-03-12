#import <Foundation/Foundation.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#import "udt.h"

using namespace UDT;

@interface OBJC_UDT : NSObject 
{ }

/*
 * Start the library
 * UDT::startup()
 */
+(int)startup;

/*
 * Releases the library
 * UDT::cleanup()
 */
+(int)cleanup;

/*
 * Creates a new UDT socket.
 * UDT::socket()
 */
+(UDTSOCKET)socket:(int)af type:(int)type protocol:(int)protocol;

/*
 * Binds a UDT socket to a known or an available local address.
 * UDT::bind()
 */
+(int)bind:(UDTSOCKET)u address:(const struct sockaddr*)name length:(int)namelen;
+(int)bind:(UDTSOCKET)u socket:(UDTSOCKET)udpsock;

/*
 * Enables a server UDT entity to wait for clients to connect.
 * UDT::listen()
 */
+(int)listen:(UDTSOCKET)u maxPending:(int)backlog;

/*
 * UDT::accept()
 */
+(UDTSOCKET)accept:(UDTSOCKET)u address:(struct sockaddr*) addr length:(int*)addrlen;

/**
 * Connects to a server socket (in regular mode) or a peer socket (in rendezvous mode) to set up a UDT connection.
 * UDT::connect()
 */
+(int)connect:(UDTSOCKET)u address:(const struct sockaddr*)name length:(int)namelen;

/*
 * Closes a UDT connection.
 * UDT::close()
 */
+(int)close:(UDTSOCKET)u;

/*
 * retrieves the address informtion of the peer side of a connected UDT socket.
 * UDT::getpeername()
 */
+(int)getpeername:(UDTSOCKET)u address:(struct sockaddr*)name length:(int*)namelen;

/*
 * retrieves the local address associated with a UDT socket.
 * UDT::getsockname()
 */
+(int)getsockname:(UDTSOCKET)u address:(struct sockaddr*)name length:(int*)namelen;

/*
 * Read and set UDT options, respectively.
 */
+(int)getsockopt:(UDTSOCKET)u level:(int)level optname:(SOCKOPT)optname optval:(void*)optval length:(int*)optlen;
+(int)setsockopt:(UDTSOCKET)u level:(int)level optname:(SOCKOPT)optname optval:(const void*)optval length:(int)optlen;

/**
 * sends out certain amount of data from an application buffer.
 * UDT::send()
 */
+(int)send:(UDTSOCKET)u buffer:(const char*) buf length:(int)len flags:(int)flags;

/**
 * reads certain amount of data into a local memory buffer.
 * UDT::recv()
 */
+(int)recv:(UDTSOCKET)u buffer:(char*)buf length:(int)len flags:(int)flags;

/**
 * sends a message to the peer side.
 * UDT::sendmsg()
 */
+(int)sendmsg:(UDTSOCKET)u buffer:(const char*) buf length:(int)len ttl:(int)ttl inorder:(bool)inorder;
+(int)sendmsg:(UDTSOCKET)u buffer:(const char*) buf length:(int)len;

/**
 * receives a valid message.
 * UDT::recvmsg()
 */
+(int)recvmsg:(UDTSOCKET)u buffer:(char*)buf length:(int)len;

/**
 * Sends out part or the whole of a local file.
 * UDT::sendfile()
 */
+(int64_t)sendfile:(UDTSOCKET)u stream:(std::fstream&)ifs offset:(int64_t)offset size:(int64_t)size block:(int)block;

/**
 * Reads certain amount of data into a local file.
 * UDT::recvfile()
 */
+(int64_t)recvfile:(UDTSOCKET)u stream:(std::fstream&)ofs offset:(int64_t)offset size:(int64_t)size block:(int)block;

/**
 * queries one or more groups of UDT sockets.
 * UDT::select()
 */
+(int)select:(int)nfds readfds:(UDSET*)readfds writefds:(UDSET*)writefds exceptfds:(UDSET*)exceptfds timeout:(const struct timeval*)timeout;
+(int)select:(UDSET*)readfds writefds:(UDSET*)writefds timeout:(const struct timeval*)timeout;
+(int)selectEx:(const std::vector<UDTSOCKET>&)fds readfds:(std::vector<UDTSOCKET>*)readfds writefds:(std::vector<UDTSOCKET>*)writefds exceptfds:(std::vector<UDTSOCKET>*)exceptfds msTimeOut:(int64_t)msTimeOut;

/**
 * last UDT error within the same thread.
 * UDT::perfmon()
 */
+(ERRORINFO&)getlasterror;

/**
 * retrieves the internal protocol parameters and performance trace.
 * UDT::perfmon()
 */
+(int)perfmon:(UDTSOCKET)u perf:(TRACEINFO*)perf clear:(bool)clear;

@end
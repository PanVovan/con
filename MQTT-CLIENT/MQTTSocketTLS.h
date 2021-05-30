#ifndef MQTTSocketTLS_H_
#define MQTTSocketTLS_H_

#include "mbed.h"

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    <wolfssl/ssl.h>          /* wolfSSL security library */
#include    <wolfssl/wolfcrypt/error-crypt.h>
#include    <wolfssl/certs_test.h>   /* memory image test certs */
#include    <user_settings.h>

#define MAXDATASIZE (1024*4)
#ifndef MBED_CONF_APP_IDLE_THREAD_STACK_SIZE
#ifdef MBED_TICKLESS
#define MBED_CONF_APP_IDLE_THREAD_STACK_SIZE 1024
#else
#define MBED_CONF_APP_IDLE_THREAD_STACK_SIZE 512
#endif
#endif

static int SocketReceive(WOLFSSL* ssl, char *buf, int sz, void *sock)
{
    int recvd;
    
    recvd = ((TCPSocket *)sock)->recv(buf, sz) ;
    if(recvd > 0)
    {
        return recvd;
    }
    else if (recvd == 0)
    {
        return WOLFSSL_CBIO_ERR_CONN_CLOSE;
    }
    else if (recvd == EWOULDBLOCK) /* for non-blocing */
    {
        return WOLFSSL_CBIO_ERR_WANT_READ;
    }
    else
    {
        return WOLFSSL_CBIO_ERR_GENERAL;
    }
}
 
static int SocketSend(WOLFSSL* ssl, char *buf, int sz, void *sock)
{
    int sent;
    sent = ((TCPSocket *)sock)->send(buf, sz);
 
    if(sent > 0) return sent;
    else if (sent == 0)
    {
        return WOLFSSL_CBIO_ERR_CONN_CLOSE;
    }
    else if (sent == EWOULDBLOCK) /* for non-blocing */
    {
        return WOLFSSL_CBIO_ERR_WANT_WRITE;
    }
    else
    {
        return WOLFSSL_CBIO_ERR_GENERAL;
    }
}

class MQTTSocketTLS: public TCPSocket
{
public:
    template <typename S>
    nsapi_error_t open(S *stack)
    {
        return _socket->open(stack);
    }

    nsapi_error_t connect(const SocketAddress &address) override
    {
        nsapi_error_t res = _socket->connect(address);
        printf("opened: %d\n", res);
        if(res == 0)
        {
            return _secure_connect() ;
        }
        return res ;
    }

    nsapi_size_or_error_t send(const void *data, nsapi_size_t size) override
    {
        return wolfSSL_write(_ssl, (char*)data, size);
    }

    nsapi_size_or_error_t recv(void *data, nsapi_size_t size) override
    {
        return wolfSSL_read(_ssl, (char*)data, size);
    }


    MQTTSocketTLS()
    {
        _socket = new TCPSocket();
    }

    ~MQTTSocketTLS() override
    {
        clear();
        delete _socket;
    }

    nsapi_size_or_error_t sendto(const SocketAddress &address,
                                 const void *data, nsapi_size_t size) override
    {
        return _socket->sendto(address, data, size);
    }

    nsapi_size_or_error_t recvfrom(SocketAddress *address,
                                   void *data, nsapi_size_t size) override
    {
        return _socket->recvfrom(address, data, size);
    }

    TCPSocket *accept(nsapi_error_t *error = NULL) override
    {
        return _socket->accept(error);
    }

    nsapi_error_t listen(int backlog = 1) override
    {
        return _socket->listen(backlog);
    }


    nsapi_error_t bind(const SocketAddress &address) override
    {
        return _socket->bind(address);
    }

    void set_blocking(bool blocking) override
    {
        return _socket->set_blocking(blocking);
    }

    void set_timeout(int timeout) override
    {
        return _socket->set_timeout(timeout);
    }

protected:
    nsapi_protocol_t get_proto() override
    {
        return NSAPI_TCP;
    }

private:
    TCPSocket *_socket;

    WOLFSSL_METHOD* _method;
    WOLFSSL_CTX* _ctx;
    WOLFSSL* _ssl;    /* create WOLFSSL object */

    const unsigned char *_cert  = ca_cert_der_2048;
    int _sizeof_cert = sizeof_ca_cert_der_2048;

    int _secure_connect()
    {
        int ret = 0;

        _method = wolfTLSv1_2_client_method();
        
        /* create and initiLize WOLFSSL_CTX structure */
        if ((_ctx = wolfSSL_CTX_new(_method)) == NULL)
        {
            printf("SSL_CTX_new error.\n");
            return EXIT_FAILURE;
        }

        wolfSSL_CTX_set_verify(_ctx, SSL_VERIFY_PEER, 0);
        wolfSSL_SetIORecv(_ctx, SocketReceive);
        wolfSSL_SetIOSend(_ctx, SocketSend);

        if ((ret = wolfSSL_CTX_load_verify_buffer(_ctx, ca_cert_der_2048,
            sizeof_ca_cert_der_2048, WOLFSSL_FILETYPE_ASN1)) != WOLFSSL_SUCCESS)
        {
            printf("can't load buffer ca file(%d)\n", ret);
            return EXIT_FAILURE;
        }

        if ((_ssl = wolfSSL_new(_ctx)) == NULL)
        {
            printf("wolfSSL_new error.\n");
            return EXIT_FAILURE;
        }

        wolfSSL_SetIOReadCtx(_ssl, (void *)_socket) ;
        wolfSSL_SetIOWriteCtx(_ssl, (void *)_socket) ;

        ret = wolfSSL_connect(_ssl);
        if (ret != SSL_SUCCESS)
        {
            ret = wolfSSL_get_error(_ssl, 0);
            printf("TLS Connect error[%d], %s\n", ret, wc_GetErrorString(ret));
            return EXIT_FAILURE;
        }
        else
        {
            return 0;
        }
    }

    void clear()
    {
        wolfSSL_shutdown(_ssl);
        wolfSSL_free(_ssl);
        wolfSSL_CTX_free(_ctx);
        wolfSSL_Cleanup();
    }
       
};
#endif
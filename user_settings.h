#include <stddef.h>

    #define WOLFSSL_CMSIS_RTOS    
    #define WOLFSSL_USER_IO
    #define NO_WRITEV
    #define NO_DEV_RANDOM
    
    #define HAVE_ECC
    #define HAVE_AESGCM
    #define WOLFSSL_GENSEED_FORTEST
    

    #define HAVE_SUPPORTED_CURVES
    #define HAVE_TLS_EXTENSIONS
    #define WC_RSA_PSS 
    #define HAVE_HKDF
    #define HAVE_FFDHE_2048
    #define TLS13_SUPPORTS_0RTT
    
    #define WOLFSSL_SHA384
    #define WOLFSSL_SHA512
    #define HAVE_CURVE25519
    #define HAVE_ED25519   /* with HAVE_SHA512 */
    #define HAVE_POLY1305
    #define HAVE_CHACHA
    #define HAVE_ONE_TIME_AUTH
    
    #define NO_SESSION_CACHE // For Small RAM
    #define NO_WOLFSSL_DIR 
     
    //#define DEBUG_WOLFSSL


    #define SIZEOF_LONG_LONG  8
    /* Options for Sample program */
    //#define WOLFSSL_NO_VERIFYSERVER


    /* Options for test */
    #define USE_CERT_BUFFERS_2048 /* use test certs */
    #define NO_ASN_TIME /* No cert date validity check */
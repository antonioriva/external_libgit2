#ifndef INCLUDE_features_h__
#define INCLUDE_features_h__

/* #undef GIT_DEBUG_POOL */
/* #undef GIT_TRACE */
#define GIT_THREADS 1
/* #undef GIT_MSVC_CRTDBG */

/* #undef GIT_ARCH_64 */
#define GIT_ARCH_32 1

/* #undef GIT_USE_ICONV */
#define GIT_USE_NSEC 1
/* #undef GIT_USE_STAT_MTIM */
/* #undef GIT_USE_STAT_MTIMESPEC */
#define GIT_USE_STAT_MTIME_NSEC 1
#define GIT_USE_FUTIMENS 1
/* #undef GIT_USE_REGCOMP_L */

#define GIT_SSH 1
/* #undef GIT_SSH_MEMORY_CREDENTIALS */

/* #undef GIT_GSSAPI */
/* #undef GIT_WINHTTP */

#define GIT_HTTPS 1
#define GIT_OPENSSL 1
/* #undef GIT_SECURE_TRANSPORT */
/* #undef GIT_MBEDTLS */

#define GIT_SHA1_COLLISIONDETECT 1
/* #undef GIT_SHA1_WIN32 */
/* #undef GIT_SHA1_COMMON_CRYPTO */
/* #undef GIT_SHA1_OPENSSL */
/* #undef GIT_SHA1_MBEDTLS */

// https://stackoverflow.com/a/32450806/314015
#define getpwuid_r(_uid, _pwd, _buf, _buflen, _result) 0

#endif

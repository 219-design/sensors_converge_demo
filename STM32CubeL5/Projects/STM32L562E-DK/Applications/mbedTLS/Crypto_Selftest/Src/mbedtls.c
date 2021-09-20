/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : mbedtls.c
  * Description        : This file provides code for the configuration
  *                      of the mbedtls instances.
  ******************************************************************************
  ******************************************************************************
   * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls.h"

/* USER CODE BEGIN 0 */
#include "main.h"
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Global variables ---------------------------------------------------------*/

/* USER CODE BEGIN 2 */
/* LCD message */
char msg[40];

/* use static allocation to keep the heap size as low as possible */
#ifdef MBEDTLS_MEMORY_BUFFER_ALLOC_C
uint8_t memory_buf[MAX_MEM_SIZE];
#endif

static int test_snprintf( size_t n, const char ref_buf[10], int ref_ret )
{
    int ret;
    char buf[10] = "xxxxxxxxx";
    const char ref[10] = "xxxxxxxxx";

    ret = mbedtls_snprintf( buf, n, "%s", "123" );
    if( ret < 0 || (size_t) ret >= n )
        ret = -1;

    if( strncmp( ref_buf, buf, sizeof( buf ) ) != 0 ||
        ref_ret != ret ||
        memcmp( buf + n, ref + n, sizeof( buf ) - n ) != 0 )
    {
        return( 1 );
    }

    return( 0 );
}

static int run_test_snprintf( void )
{
    return( test_snprintf( 0, "xxxxxxxxx",  -1 ) != 0 ||
            test_snprintf( 1, "",           -1 ) != 0 ||
            test_snprintf( 2, "1",          -1 ) != 0 ||
            test_snprintf( 3, "12",         -1 ) != 0 ||
            test_snprintf( 4, "123",         3 ) != 0 ||
            test_snprintf( 5, "123",         3 ) != 0 );
}

/*
 * Check if a seed file is present, and if not create one for the entropy
 * self-test. If this fails, we attempt the test anyway, so no error is passed
 * back.
 */
#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_ENTROPY_C)
#if defined(MBEDTLS_ENTROPY_NV_SEED) && !defined(MBEDTLS_NO_PLATFORM_ENTROPY)
static void create_entropy_seed_file( void )
{
    int result;
    size_t output_len = 0;
    unsigned char seed_value[MBEDTLS_ENTROPY_BLOCK_SIZE];

    /* Attempt to read the entropy seed file. If this fails - attempt to write
     * to the file to ensure one is present. */
    result = mbedtls_platform_std_nv_seed_read( seed_value,
                                                    MBEDTLS_ENTROPY_BLOCK_SIZE );
    if( 0 == result )
        return;

    result = mbedtls_platform_entropy_poll( NULL,
                                            seed_value,
                                            MBEDTLS_ENTROPY_BLOCK_SIZE,
                                            &output_len );
    if( 0 != result )
        return;

    if( MBEDTLS_ENTROPY_BLOCK_SIZE != output_len )
        return;

    mbedtls_platform_std_nv_seed_write( seed_value, MBEDTLS_ENTROPY_BLOCK_SIZE );
}
#endif

static int mbedtls_entropy_self_test_wrapper( int verbose )
{
#if defined(MBEDTLS_ENTROPY_NV_SEED) && !defined(MBEDTLS_NO_PLATFORM_ENTROPY)
    create_entropy_seed_file( );
#endif
    return( mbedtls_entropy_self_test( verbose ) );
}
#endif

#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
int mbedtls_memory_buffer_alloc_free_and_self_test( int verbose )
{
    if( verbose != 0 )
    {
#if defined(MBEDTLS_MEMORY_DEBUG)
        mbedtls_memory_buffer_alloc_status( );
#endif
    }
    mbedtls_memory_buffer_alloc_free( );
    return( mbedtls_memory_buffer_alloc_self_test( verbose ) );
}
#endif

typedef struct
{
    const char *name;
    int ( *function )( int );
} selftest_t;

const selftest_t selftests[] =
{
#if defined(MBEDTLS_MD2_C)
    {"md2", mbedtls_md2_self_test},
#endif
#if defined(MBEDTLS_MD4_C)
    {"md4", mbedtls_md4_self_test},
#endif
#if defined(MBEDTLS_MD5_C)
    {"md5", mbedtls_md5_self_test},
#endif
#if defined(MBEDTLS_RIPEMD160_C)
    {"ripemd160", mbedtls_ripemd160_self_test},
#endif
#if defined(MBEDTLS_SHA1_C)
    {"sha1", mbedtls_sha1_self_test},
#endif
#if defined(MBEDTLS_SHA256_C)
    {"sha256", mbedtls_sha256_self_test},
#endif
#if defined(MBEDTLS_SHA512_C)
    {"sha512", mbedtls_sha512_self_test},
#endif
#if defined(MBEDTLS_ARC4_C)
    {"arc4", mbedtls_arc4_self_test},
#endif
#if defined(MBEDTLS_DES_C)
    {"des", mbedtls_des_self_test},
#endif
#if defined(MBEDTLS_AES_C)
    {"aes", mbedtls_aes_self_test},
#endif
#if defined(MBEDTLS_GCM_C) && defined(MBEDTLS_AES_C)
    {"gcm", mbedtls_gcm_self_test},
#endif
#if defined(MBEDTLS_CCM_C) && defined(MBEDTLS_AES_C)
    {"ccm", mbedtls_ccm_self_test},
#endif
#if defined(MBEDTLS_NIST_KW_C) && defined(MBEDTLS_AES_C)
    {"nist_kw", mbedtls_nist_kw_self_test},
#endif
#if defined(MBEDTLS_CMAC_C)
    {"cmac", mbedtls_cmac_self_test},
#endif
#if defined(MBEDTLS_CHACHA20_C)
    {"chacha20", mbedtls_chacha20_self_test},
#endif
#if defined(MBEDTLS_POLY1305_C)
    {"poly1305", mbedtls_poly1305_self_test},
#endif
#if defined(MBEDTLS_CHACHAPOLY_C)
    {"chacha20-poly1305", mbedtls_chachapoly_self_test},
#endif
#if defined(MBEDTLS_BASE64_C)
    {"base64", mbedtls_base64_self_test},
#endif
#if defined(MBEDTLS_BIGNUM_C)
    {"mpi", mbedtls_mpi_self_test},
#endif
#if defined(MBEDTLS_RSA_C)
    {"rsa", mbedtls_rsa_self_test},
#endif
#if defined(MBEDTLS_X509_USE_C)
    {"x509", mbedtls_x509_self_test},
#endif
#if defined(MBEDTLS_XTEA_C)
    {"xtea", mbedtls_xtea_self_test},
#endif
#if defined(MBEDTLS_CAMELLIA_C)
    {"camellia", mbedtls_camellia_self_test},
#endif
#if defined(MBEDTLS_ARIA_C)
    {"aria", mbedtls_aria_self_test},
#endif
#if defined(MBEDTLS_CTR_DRBG_C)
    {"ctr_drbg", mbedtls_ctr_drbg_self_test},
#endif
#if defined(MBEDTLS_HMAC_DRBG_C)
    {"hmac_drbg", mbedtls_hmac_drbg_self_test},
#endif
#if defined(MBEDTLS_ECP_C)
    {"ecp", mbedtls_ecp_self_test},
#endif
#if defined(MBEDTLS_ECJPAKE_C)
    {"ecjpake", mbedtls_ecjpake_self_test},
#endif
#if defined(MBEDTLS_DHM_C)
    {"dhm", mbedtls_dhm_self_test},
#endif
#if defined(MBEDTLS_ENTROPY_C)
    {"entropy", mbedtls_entropy_self_test_wrapper},
#endif
#if defined(MBEDTLS_PKCS5_C)
    {"pkcs5", mbedtls_pkcs5_self_test},
#endif
/* Slower test after the faster ones */
#if defined(MBEDTLS_TIMING_C)
    {"timing", mbedtls_timing_self_test},
#endif
/* Heap test comes last */
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
    {"memory_buffer_alloc", mbedtls_memory_buffer_alloc_free_and_self_test},
#endif

    {NULL, NULL}
};
#endif /* MBEDTLS_SELF_TEST */
/* USER CODE END 2 */

/* MBEDTLS init function */
void MX_MBEDTLS_Init(void)
{
   /**
  */
  /* USER CODE BEGIN 3 */
  char mbedtls_string_version[20];

  mbedtls_version_get_string_full( mbedtls_string_version );

  sprintf(msg,"  Running with %s", mbedtls_string_version);
  UTIL_LCD_DisplayStringAt(0, 50, (uint8_t *)msg, LEFT_MODE);

  mbedtls_printf( "\nSelftest Application running with %s\n", mbedtls_string_version );

  mbedtls_printf( "\n" );
  /* USER CODE END 3 */

}

/* USER CODE BEGIN 4 */
void mbedTLS_Selftest(void)
{
    int suites_tested = 0, suites_failed = 0;
#if defined(MBEDTLS_SELF_TEST)
    int v = 1; /* v=1 for verbose mode */
    const selftest_t *test;
#endif /* MBEDTLS_SELF_TEST */
    void *pointer;
      /*
     * The C standard doesn't guarantee that all-bits-0 is the representation
     * of a NULL pointer. We do however use that in our code for initializing
     * structures, which should work on every modern platform. Let's be sure.
     */
    memset( &pointer, 0, sizeof( void * ) );
    if( pointer != NULL )
    {
        mbedtls_printf( "all-bits-zero is not a NULL pointer\n" );
        goto exit;
    }

    /*
     * Make sure we have a snprintf that correctly zero-terminates
     */
    if( run_test_snprintf() != 0 )
    {
        mbedtls_printf( "the snprintf implementation is broken\n" );
        goto exit;
    }

#if defined(MBEDTLS_SELF_TEST)
#ifdef MBEDTLS_MEMORY_BUFFER_ALLOC_C
    mbedtls_memory_buffer_alloc_init( memory_buf, sizeof(memory_buf) );
#endif

/* Set your alternate threading implementation */
#if defined(MBEDTLS_THREADING_C)
#ifdef MBEDTLS_THREADING_ALT
    mbedtls_threading_set_alt( cmsis_os_mutex_init,
                               cmsis_os_mutex_free,
                               cmsis_os_mutex_lock,
                               cmsis_os_mutex_unlock );
#endif /* MBEDTLS_THREADING_ALT */
#endif /* MBEDTLS_THREADING_C */

    /* Run all the tests */
    for( test = selftests; test->name != NULL; test++ )
    {
        if( test->function( v )  != 0 )
        {
            suites_failed++;
        }
        suites_tested++;
    }

    /* verbose on hyperterminal and LCD */
    if( v != 0 )
    {
        mbedtls_printf( "  Executed %d test suites\n\n", suites_tested );

        sprintf(msg,"  Executed %d test suites", suites_tested);
        UTIL_LCD_DisplayStringAt(0, 80, (uint8_t *)msg, LEFT_MODE);

        if( suites_failed > 0)
        {
            mbedtls_printf( "  [ %d tests FAIL ]\n\n", suites_failed );

            sprintf(msg,"  [ %d tests FAIL]",suites_failed);
            UTIL_LCD_DisplayStringAt(0, 100, (uint8_t *)msg, LEFT_MODE);
        }
        else
        {
            mbedtls_printf( "  [ All tests PASS ]\n\n" );
            UTIL_LCD_DisplayStringAt(0, 100, (uint8_t *)"[ All tests PASS ]", LEFT_MODE);
        }
    }

#if defined(MBEDTLS_THREADING_C)
#ifdef MBEDTLS_THREADING_ALT
  mbedtls_threading_free_alt();
#endif /* MBEDTLS_THREADING_ALT */
#endif /* MBEDTLS_THREADING_C */

#else
    mbedtls_printf( " MBEDTLS_SELF_TEST not defined.\n" );
    goto exit;
#endif /* MBEDTLS_SELF_TEST */

exit:
    if( ( suites_failed > 0 ) || ( suites_tested == 0 ) )
   {
       Error_Handler();
   }
}
/* USER CODE END 4 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

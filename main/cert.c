/* This is the CA certificate for the CA trust chain of
   www.howsmyssl.com in PEM format, as dumped via:

   openssl s_client -showcerts -connect abc.cloudmqtt.com:12345 </dev/null

   The CA cert is the last cert in the chain output by the server.
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 1 s:/C=US/O=Let's Encrypt/CN=Let's Encrypt Authority X3
   i:/O=Digital Signature Trust Co./CN=DST Root CA X3
 */
const char *server_root_cert = "-----BEGIN CERTIFICATE-----\r\n"
"MIIDqjCCApKgAwIBAgIJAKTijmXipWe4MA0GCSqGSIb3DQEBDQUAMGoxFzAVBgNV\r\n"
"BAMMDkFuIE1RVFQgYnJva2VyMRYwFAYDVQQKDA1Pd25UcmFja3Mub3JnMRQwEgYD\r\n"
"VQQLDAtnZW5lcmF0ZS1DQTEhMB8GCSqGSIb3DQEJARYSbm9ib2R5QGV4YW1wbGUu\r\n"
"bmV0MB4XDTE4MDUyNTA1NTcyOFoXDTMyMDUyMTA1NTcyOFowajEXMBUGA1UEAwwO\r\n"
"QW4gTVFUVCBicm9rZXIxFjAUBgNVBAoMDU93blRyYWNrcy5vcmcxFDASBgNVBAsM\r\n"
"C2dlbmVyYXRlLUNBMSEwHwYJKoZIhvcNAQkBFhJub2JvZHlAZXhhbXBsZS5uZXQw\r\n"
"ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCmRJ/6gjiGzlFZwNZQa0ns\r\n"
"Osm18GH/nhZN4IEWP8RHmRy5a5dnrKr0bI808O1P8VkN9pYjod4Rmzp/LgsknkXb\r\n"
"bsgPIgxZjiqJTqf33MsZ9dUjCidYnlgfpmLG4ITIryUVr/EtaT2Hw5LCp3J0Fuge\r\n"
"o1wTjCgoA87l80sVNKDImx5thGZaMWPyU5bSiNar8dj3szVuYi2nQCqh2c0Auu/d\r\n"
"cgvbmJi0jQ+Jpivy5XerakUV8A8gAzG3iSfQqC+pW2OxA2ENIhCjh/ML/CwHRWdG\r\n"
"7gSeVPoem6Lru8oTO2/x2uSalkusKzRNqiVmj3Q62p6KFMe6/esKjdbr1v/smvG7\r\n"
"AgMBAAGjUzBRMB0GA1UdDgQWBBSxn/v8BZGrN4aMcpaZeFMYB/DS3DAfBgNVHSME\r\n"
"GDAWgBSxn/v8BZGrN4aMcpaZeFMYB/DS3DAPBgNVHRMBAf8EBTADAQH/MA0GCSqG\r\n"
"SIb3DQEBDQUAA4IBAQAlgYzMI4S/0qD/vLYy/KM/x0P3hK2wknGLtEfJOhXF1mBO\r\n"
"PSpXLmCTjFpQ+UGMU7lk1A02HmD/SopVK9rLLdbApbKXD9pkmYskxVi3pIxLZ5HV\r\n"
"alCbHDS+KKlKovYfXJQRILiEqjZhkOpdqMItgPvD1TpO5cvEkpSyPB7cXe9CrHSS\r\n"
"XbGL+wnchNd61IJhxl2jEkHTojSP5YHzI2NS2VoJ/U3aM2Bw4iPtgoo3rYQq+Rf/\r\n"
"DasyCn1mJFj5Tk77KhOlH4+D+UuQptq33yI1EkngzBxwmozY1kxtHoYdI/nr9sRJ\r\n"
"KYn35MLJ15X8VZi9fovbkS8MtcTp21f/rUGscW+J\r\n"
"-----END CERTIFICATE-----\r\n";



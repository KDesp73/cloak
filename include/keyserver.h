#ifndef KEY_SERVER_H
#define KEY_SERVER_H

#include <stddef.h>
typedef struct {
    // Function pointer to request the encrypted AES key for a user
    int (*request_key)(const char* user_public_key, unsigned char *encrypted_key, size_t *encrypted_key_len);

    // Function pointer to store the encrypted AES key for a user
    int (*store_key)(const char* user_private_key, const unsigned char *encrypted_key, size_t encrypted_key_len);
} CLOAK_KeyServer;


#endif // KEY_SERVER_H

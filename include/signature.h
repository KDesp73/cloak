#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "context.h"
#include "rsa.h"
#define CLOAK_SIGNATURE_FILE "cloak.sig"

typedef unsigned char CLOAK_Signature[CLOAK_RSA_SIGNATURE_SIZE];

typedef struct {
    CLOAK_Signature* items;
    size_t count;
} CLOAK_SignatureList;

void CLOAK_LoadSignatures(CLOAK_SignatureList* signatures);
void CLOAK_ValidateSignatures(const CLOAK_SignatureList* signatures, const CLOAK_Context* ctx);
void CLOAK_FreeSignatures(CLOAK_SignatureList* signatures);


#ifdef CLOAK_REMOVE_PREFIXES
#define SIGNATURE_FILE CLOAK_SIGNATURE_FILE
#define Signature CLOAK_Signature
#define SignatureList CLOAK_SignatureList
#define LoadSignatures CLOAK_LoadSignatures
#define ValidateSignatures CLOAK_ValidateSignatures
#define FreeSignatures CLOAK_FreeSignatures
#endif // CLOAK_REMOVE_PREFIXES

#endif // SIGNATURE_H

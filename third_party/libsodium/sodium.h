#ifndef SODIUM_STUB_H
#define SODIUM_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal libsodium API used by Flinstone OS */

#define crypto_pwhash_STRBYTES 128
#define crypto_pwhash_OPSLIMIT_MODERATE 2
#define crypto_pwhash_MEMLIMIT_MODERATE (1 << 20)

int sodium_init(void);

int crypto_pwhash_str(char out[crypto_pwhash_STRBYTES], const char *passwd,
                      unsigned long long passwdlen,
                      unsigned long long opslimit,
                      size_t memlimit);

int crypto_pwhash_str_verify(const char *hashed_passwd, const char *passwd,
                             unsigned long long passwdlen);

#ifdef __cplusplus
}
#endif

#endif /* SODIUM_STUB_H */


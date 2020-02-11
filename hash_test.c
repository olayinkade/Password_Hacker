#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#define _GNU_SOURCE


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>

#include "hash_test.h"


// adapted from from the GNU libc manual
// https://www.gnu.org/software/libc/manual/html_node/crypt.html
bool same(char* pass, char* hash)
{
  char *result;
  int ok;

  struct crypt_data data;
  data.initialized = 0;

  result = crypt_r(pass, hash, (struct crypt_data*)&data);
  /* Test the result. */
  return strcmp (result, hash) == 0;
}

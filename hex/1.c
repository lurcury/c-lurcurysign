#include <stdlib.h>
#include <stdio.h>

unsigned char * bin_to_strhex(const unsigned char *bin, unsigned int binsz,
                                  unsigned char **result)
{
  unsigned char     hex_str[]= "0123456789abcdef";
  unsigned int      i;

  if (!(*result = (unsigned char *)malloc(binsz * 2 + 1)))
    return (NULL);

  (*result)[binsz * 2] = 0;

  if (!binsz)
    return (NULL);

  for (i = 0; i < binsz; i++)
    {
      (*result)[i * 2 + 0] = hex_str[(bin[i] >> 4) & 0x0F];
      (*result)[i * 2 + 1] = hex_str[(bin[i]     ) & 0x0F];
    }
  return (*result);
}

int main()
{
  //the calling
  unsigned char     buf[] = {0,1,10,11};
  unsigned char *   result;

  printf("result : %s\n", bin_to_strhex((unsigned char *)buf, sizeof(buf), &result));
  free(result);

  return 0;
}

uint8_t buf[] = {0, 1, 10, 11};

/* Allocate twice the number of the bytes in the buf array because each byte would be 
 * converted to two hex characters, also add an extra space for the terminating null byte
 * [size] is the size of the buf array */
char output[(size * 2) + 1];

/* pointer to the first item (0 index) of the output array */
char *ptr = &output[0];

int i;

for (i = 0; i < size; i++)
{
    /* sprintf converts each byte to 2 chars hex string and a null byte, for example
     * 10 => "0A\0".
     *
     * These three chars would be added to the output array starting from
     * the ptr location, for example if ptr is pointing at 0 index then the hex chars
     * "0A\0" would be written as output[0] = '0', output[1] = 'A' and output[2] = '\0'.
     *
     * sprintf returns the number of chars written execluding the null byte, in our case
     * this would be 2. Then we move the ptr location two steps ahead so that the next
     * hex char would be written just after this one and overriding this one's null byte.
     *
     * We don't need to add a terminating null byte because it's already added from 
     * the last hex string. */  
    ptr += sprintf (ptr, "%02X", buf[i]);
}

printf ("%s\n", output);

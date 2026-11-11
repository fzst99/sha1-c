#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t ROTL(uint32_t object , unsigned amount) {

    amount %= 32;
    if (amount == 0) {
        return object;
    }

    return (object << amount) | (object >> (32-amount)) ;

}

uint32_t ROTR(uint32_t object , unsigned amount) {

    amount %= 32;

    if (amount==0) {
        return object;
    }

    return (object >> amount) | (object << (32-amount));

}

char *turn_back_to_binary(char *character){

    int ascii_value = *character ;
    int power_two = 128 ;
    int power_index = 8;

    char *binary = malloc(9*sizeof(char));

    int i=0;

    if (binary == NULL) {
        printf("memory allocation failed...");
        return NULL;
    }

    while(power_index > 0) {

        if (ascii_value >= power_two) {

            binary[i++] = '1';
            ascii_value -= power_two;
            power_two /= 2;
            power_index -= 1;

        }

        else{

            binary[i++] = '0';
            power_two /= 2;
            power_index -= 1;

        }

    }

    binary[8]='\0';

    return binary;

}

char *converting_to_binary(char *string) {

    int index=0 ;

    char *full_text_in_binary = malloc(strlen(string)*8 +1);

    if (full_text_in_binary == NULL) {

        printf("memory allocation faield...");
        return NULL;

    }

    while (string[index] != '\0') {

        char character = string[index];

        char *temp = turn_back_to_binary(&character);

        if (temp==NULL) {

            printf("memory allocation failed...");
            free(full_text_in_binary);
            return NULL;

        }

        memcpy(full_text_in_binary + index * 8,temp,8);

        free(temp);

        index++;

    }

    full_text_in_binary[index *8] = '\0';

    return full_text_in_binary;

}

char *convert_length_to_64(uint64_t *length) {

    uint64_t power_two = UINT64_C(9223372036854775808);

    int power_index = 64;

    char *binary = malloc(65*sizeof(char));

    if (binary == NULL) {

        printf("memory allocation failed...");
        return NULL;

    }

    int b_index=0;

    while (power_index > 0) {

        if (*length >= power_two) {

            binary[b_index++] = '1';
            *length -= power_two;
            power_two /= 2;
            power_index -= 1;

        }

        else {

            binary[b_index++] = '0';
            power_two /= 2;
            power_index -= 1;

        }

    }

    binary[64] = '\0';

    return binary;

}

char *padding(char *message_bits, uint64_t length_in_bits) {

    char *length_in_64 = convert_length_to_64(&length_in_bits);

    if (length_in_64 == NULL) {
        return NULL;
    }

    int indexing=0;
    int capacity = 18 ;

    char *everything_glued = malloc(513 * sizeof(char));

    if (everything_glued == NULL) {

        printf("memory allocation failed...");
        free(length_in_64);
        return NULL;

    }

    while (indexing < strlen(message_bits)) {

        everything_glued[indexing] = message_bits[indexing] ;
        indexing++;

    }

    everything_glued[indexing] = '1';
    indexing++;

    int stored_index = indexing; 

    indexing=0;

    while (indexing < 64){

        everything_glued[stored_index+indexing] = length_in_64[indexing];
        indexing++;

    }


    int glued_length = strlen(message_bits) + 1 + 64;

    everything_glued[glued_length]='\0';

    char *final_padded_bits = malloc(513);

    if (final_padded_bits == NULL) {

        printf("memory allocation failed...");
        free(everything_glued);
        free(length_in_64);
        return NULL;

    }

    int number_ofzeros;

    if (glued_length >= 512) {

        number_ofzeros = 512 - (glued_length % 512);

        /*
         * This implementation currently returns one 512-bit block.
         * For messages that need multiple blocks, padding must later
         * be extended to allocate and return multiple blocks.
         */

        if (glued_length > 512) {

            printf("message too long for current single-block padding implementation\n");

            free(everything_glued);
            free(length_in_64);
            free(final_padded_bits);

            return NULL;

        }

    }

    number_ofzeros = 512-1-64-strlen(message_bits);

    int i=0;

    for (i=0;i<strlen(message_bits);i++) {

        final_padded_bits[i]=message_bits[i];

    }

    final_padded_bits[i]='1';
    i++;

    for (int temp=0;temp < number_ofzeros;temp++,i++){

        final_padded_bits[i]='0';

    }

    for (int j=0;j<64;j++,i++){

        final_padded_bits[i]=length_in_64[j];

    }

    final_padded_bits[512]='\0';


    free(everything_glued);
    free(length_in_64);

    return final_padded_bits;

}

uint32_t *extract_words(char *final_padded_bits) {

    uint32_t *Words = malloc(16 * sizeof(uint32_t));

    if (Words==NULL) {

        printf("memory allocation failed...");
        return NULL;

    }

    int i=0;
    
    int logged_words = 0;

    char word_int_inbits[32];

    for (i=0;i< 16;i++) {

        for(int j=0;j<32;j++) {

            word_int_inbits[j]=final_padded_bits[j+(logged_words*32)];

        } 

        logged_words++;


        int index=0;

        uint32_t total = 0;

        for (index=0;index < 32 ; index ++) {

            total = total << 1;

            if (word_int_inbits[index] == '1'){

                total += 1;

            }

        }

        Words[i]=total;

    }

    return Words;

}

uint32_t *extend_words_list(uint32_t *Words) {

    uint32_t *W = malloc(80*sizeof(uint32_t));

    if(W==NULL) {

        printf("memory allocation failed...");
        return NULL;

    }

    for(int t=0;t<16;t++) {

        W[t]=Words[t];

    }

    int t=0;

    for(t=16;t<80;t++) {

        W[t] = ROTL(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16] , 1);

    }

    return W;

}

uint32_t *sha1_compression(uint32_t *W) {

    uint32_t a = 0x67452301;
    uint32_t b = 0xEFCDAB89;
    uint32_t c = 0x98BADCFE;
    uint32_t d = 0x10325476;
    uint32_t e = 0xC3D2E1F0;

    uint32_t H0 = 0x67452301;
    uint32_t H1 = 0xEFCDAB89;
    uint32_t H2 = 0x98BADCFE ;
    uint32_t H3 = 0x10325476;
    uint32_t H4 = 0xC3D2E1F0;

    for(unsigned round = 0;round < 80 ; round++) {


        uint32_t k;
        uint32_t f;

        if(round<= 19 ) {

            k = 0x5A827999;
            f = (H1 & H2) | (~H1 & H3) ;

        }

        else if(round<= 39 ) {

            k = 0x6ED9EBA1;
            f = H1 ^ H2 ^ H3;

        }

        else if (round<= 59 ) {

            k = 0x8F1BBCDC;
            f = (H1 & H2) | (H1 & H3) | (H2 & H3);

        }

        else if(round<= 79) {

            k = 0xCA62C1D6;
            f = H1 ^ H2 ^ H3;

        }

        uint32_t TEMP = ROTL(H0 , 5) + f + H4 + W[round] + k; 

        H4 = H3;
        H3 = H2;
        H2 = ROTL(H1 , 30);
        H1 = H0;
        H0 = TEMP;

    }

    H0 += a;
    H1 += b;
    H2 += c;
    H3 += d;
    H4 += e;

    uint32_t *digest = malloc(5 * sizeof(uint32_t));

    if(digest==NULL) {

        printf("memory allocation failed...");
        return NULL;

    }

    digest[0] = H0;
    digest[1] = H1;
    digest[2] = H2;
    digest[3] = H3;
    digest[4] = H4;

    return digest;

}

char *final_hashed(uint32_t *digest) {

    char *hash = malloc(41);

    if(hash==NULL) {

        printf("memory allocation failed...");
        return NULL;

    }

    int position=0;

    for (int i=0; i < 5;i++) {

        for (int shift=28;shift >= 0;shift -=4) {

            int value = (digest[i] >> shift) & 0xF;

            if (value < 10) {

                hash[position] = '0' +value;

            }

            else if (value >= 10) {

                hash[position] = 'A' + (value - 10);

            }

            position++;

        }

    }

    hash[40]='\0';

    return hash;

}

int main(int argc,char **argv) {

    if (argc < 2) {

        printf("please provide a string...");
        return 1;

    }

    size_t length = strlen(argv[1]);

    char *string = malloc(length+1);

    if (string == NULL) {

        printf("memory allocation failed !");
        return 1;

    }

    strcpy(string , argv[1]);

    char *string_in_binary = converting_to_binary(string);

    if (string_in_binary == NULL) {

        free(string);
        return 1;

    }

    uint64_t string_length = strlen(string) * 8;

    char *string_length_64 = convert_length_to_64(&string_length);

    if (string_length_64 == NULL) {

        free(string);
        free(string_in_binary);
        return 1;

    }

    char *padded_bits = padding(string_in_binary, strlen(string) * 8);

    if (padded_bits == NULL) {

        free(string);
        free(string_in_binary);
        free(string_length_64);
        return 1;

    }

    uint32_t *Words = extract_words(padded_bits);

    if (Words == NULL) {

        free(string);
        free(string_in_binary);
        free(string_length_64);
        free(padded_bits);
        return 1;

    }

    uint32_t *W = extend_words_list(Words);

    if (W == NULL) {

        free(string);
        free(string_in_binary);
        free(string_length_64);
        free(padded_bits);
        free(Words);
        return 1;

    }

    uint32_t *digest = sha1_compression(W);

    if (digest == NULL) {

        free(string);
        free(string_in_binary);
        free(string_length_64);
        free(padded_bits);
        free(Words);
        free(W);
        return 1;

    }

    char *hash = final_hashed(digest);

    if (hash == NULL) {

        free(string);
        free(string_in_binary);
        free(string_length_64);
        free(padded_bits);
        free(Words);
        free(W);
        free(digest);
        return 1;

    }

    printf("%s\n", hash);

    free(string);
    free(string_in_binary);
    free(string_length_64);
    free(padded_bits);
    free(Words);
    free(W);
    free(digest);
    free(hash);

    return 0;

}
#define MAX_STR_LENGTH_BYTES 123             // max size of the input string

#if MAX_STR_LENGTH_BYTES%4 > 0
    #define N_BUFFER_sz MAX_STR_LENGTH_BYTES/4 + 1  // max number of unsigned int 
#else
    #define N_BUFFER_sz MAX_STR_LENGTH_BYTES/4
#endif
//

#define CH(x,y,z)   (bitselect(z,y,x))            //+
#define MAJ(x,y,z)   (bitselect (x, y, (x ^ z)))  //+
#define mod(x,y) x - (x/y * y)                    //+
#define shr32(x,n) ((x) >> (n))                   //+
#define rotl32(a,n) rotate ((a), (n))             //+

#define SWAP(val) (rotate (val & 0x00FF00FF, 24U)|rotate(val & 0xFF00FF00, 8U))


#define SIGMA0(x) (rotl32 ((x), 25u) ^ rotl32 ((x), 14u) ^ shr32 ((x),  3u))//+
#define SIGMA1(x) (rotl32 ((x), 15u) ^ rotl32 ((x), 13u) ^ shr32 ((x), 10u))//+
#define SUM0(x) (rotl32 ((x), 30u) ^ rotl32 ((x), 19u) ^ rotl32 ((x), 10u)) //+
#define SUM1(x) (rotl32 ((x), 26u) ^ rotl32 ((x), 21u) ^ rotl32 ((x),  7u)) //+


#define SHA256C00 0x428a2f98u//+
#define SHA256C01 0x71374491u//+
#define SHA256C02 0xb5c0fbcfu//+
#define SHA256C03 0xe9b5dba5u//+
#define SHA256C04 0x3956c25bu//+
#define SHA256C05 0x59f111f1u//+
#define SHA256C06 0x923f82a4u//+
#define SHA256C07 0xab1c5ed5u//+

#define SHA256C08 0xd807aa98u//+
#define SHA256C09 0x12835b01u//+
#define SHA256C0a 0x243185beu//+
#define SHA256C0b 0x550c7dc3u//+
#define SHA256C0c 0x72be5d74u//+
#define SHA256C0d 0x80deb1feu//+
#define SHA256C0e 0x9bdc06a7u//+
#define SHA256C0f 0xc19bf174u//+

#define SHA256C10 0xe49b69c1u//+
#define SHA256C11 0xefbe4786u//+
#define SHA256C12 0x0fc19dc6u//+
#define SHA256C13 0x240ca1ccu//+
#define SHA256C14 0x2de92c6fu//+
#define SHA256C15 0x4a7484aau//+
#define SHA256C16 0x5cb0a9dcu//+
#define SHA256C17 0x76f988dau//+

#define SHA256C18 0x983e5152u//+
#define SHA256C19 0xa831c66du//+
#define SHA256C1a 0xb00327c8u//+
#define SHA256C1b 0xbf597fc7u//+
#define SHA256C1c 0xc6e00bf3u//+
#define SHA256C1d 0xd5a79147u//+
#define SHA256C1e 0x06ca6351u//+
#define SHA256C1f 0x14292967u//+

#define SHA256C20 0x27b70a85u//+
#define SHA256C21 0x2e1b2138u//+
#define SHA256C22 0x4d2c6dfcu//+
#define SHA256C23 0x53380d13u//+
#define SHA256C24 0x650a7354u//+
#define SHA256C25 0x766a0abbu//+
#define SHA256C26 0x81c2c92eu//+
#define SHA256C27 0x92722c85u//+

#define SHA256C28 0xa2bfe8a1u//+
#define SHA256C29 0xa81a664bu//+
#define SHA256C2a 0xc24b8b70u//+
#define SHA256C2b 0xc76c51a3u//+
#define SHA256C2c 0xd192e819u//+
#define SHA256C2d 0xd6990624u//+
#define SHA256C2e 0xf40e3585u//+
#define SHA256C2f 0x106aa070u//+

#define SHA256C30 0x19a4c116u//+
#define SHA256C31 0x1e376c08u//+
#define SHA256C32 0x2748774cu//+
#define SHA256C33 0x34b0bcb5u//+
#define SHA256C34 0x391c0cb3u//+
#define SHA256C35 0x4ed8aa4au//+
#define SHA256C36 0x5b9cca4fu//+
#define SHA256C37 0x682e6ff3u//+

#define SHA256C38 0x748f82eeu//+
#define SHA256C39 0x78a5636fu//+
#define SHA256C3a 0x84c87814u//+
#define SHA256C3b 0x8cc70208u//+
#define SHA256C3c 0x90befffau//+
#define SHA256C3d 0xa4506cebu//+
#define SHA256C3e 0xbef9a3f7u//+
#define SHA256C3f 0xc67178f2u//+

__constant uint k_sha256[64] =//+
{
  SHA256C00, SHA256C01, SHA256C02, SHA256C03, SHA256C04, SHA256C05, SHA256C06, SHA256C07,
  SHA256C08, SHA256C09, SHA256C0a, SHA256C0b, SHA256C0c, SHA256C0d, SHA256C0e, SHA256C0f,
  SHA256C10, SHA256C11, SHA256C12, SHA256C13, SHA256C14, SHA256C15, SHA256C16, SHA256C17,
  SHA256C18, SHA256C19, SHA256C1a, SHA256C1b, SHA256C1c, SHA256C1d, SHA256C1e, SHA256C1f,
  SHA256C20, SHA256C21, SHA256C22, SHA256C23, SHA256C24, SHA256C25, SHA256C26, SHA256C27,
  SHA256C28, SHA256C29, SHA256C2a, SHA256C2b, SHA256C2c, SHA256C2d, SHA256C2e, SHA256C2f,
  SHA256C30, SHA256C31, SHA256C32, SHA256C33, SHA256C34, SHA256C35, SHA256C36, SHA256C37,
  SHA256C38, SHA256C39, SHA256C3a, SHA256C3b, SHA256C3c, SHA256C3d, SHA256C3e, SHA256C3f,
};

// SHA256_STEP(MAJa,CHa,a,b,c,d,e,f,g,h,x,K) == h= K + x==w<t> + SUM1 + CHa + SUM0 + Maj
#define SHA256_STEP(MAJa,CHa,a,b,c,d,e,f,g,h,T12,x,K)  \
{\
  T12 = h + SUM1 (e) + CHa (e,f,g) + K + x;/*T1*/\
  \
  h = g;\
  g = f;\
  f = e;\
  e = d + T12;/*T1*/\
  d = c;\
  T12 += SUM0 (a) + MAJa (a,b,c);/*T1 + T2*/\
  c = b;\
  b = a;\
  a = T12;\
}

// calculate w<t> for t=16..63
#define SHA256_EXPAND(x,y,z,w) (SIGMA1 (x) + y + SIGMA0 (z) + w)

static void sha256_process2 (const unsigned int *W, unsigned int *digest)
{
  // update constants as H<i - 1>
  unsigned int a = digest[0];
  unsigned int b = digest[1];
  unsigned int c = digest[2];
  unsigned int d = digest[3];
  unsigned int e = digest[4];
  unsigned int f = digest[5];
  unsigned int g = digest[6];
  unsigned int h = digest[7];
  unsigned int T12 = 0;

  // Set W<t> = M(i)<t>, t=0..15
  unsigned int w0_t = W[0];
  unsigned int w1_t = W[1];
  unsigned int w2_t = W[2];
  unsigned int w3_t = W[3];
  unsigned int w4_t = W[4];
  unsigned int w5_t = W[5];
  unsigned int w6_t = W[6];
  unsigned int w7_t = W[7];
  unsigned int w8_t = W[8];
  unsigned int w9_t = W[9];
  unsigned int wa_t = W[10];
  unsigned int wb_t = W[11];
  unsigned int wc_t = W[12];
  unsigned int wd_t = W[13];
  unsigned int we_t = W[14];
  unsigned int wf_t = W[15];

  

  // calculate w<t> = SIGMA1(W<t-2>) + W<t-7> + SIGMA0(W<t-15>) + W<t-16>
  // for t=16..63 with step 16
  // +
  #define ROUND_EXPAND(i)                           \
  {                                                 \
    /*(0, ('0xe', '0x9', '0x1', '0x0'))+*/\
    w0_t = SHA256_EXPAND (we_t,  w9_t,  w1_t, w0_t);  \
    /*(1, ('0xf', '0xa', '0x2', '0x1'))+*/\
    w1_t = SHA256_EXPAND (wf_t,  wa_t, w2_t, w1_t);  \
    w2_t = SHA256_EXPAND (w0_t,  wb_t, w3_t, w2_t);  \
    w3_t = SHA256_EXPAND (w1_t,  wc_t, w4_t, w3_t);  \
    w4_t = SHA256_EXPAND (w2_t,  wd_t, w5_t, w4_t);  \
    w5_t = SHA256_EXPAND (w3_t,  we_t, w6_t, w5_t);  \
    w6_t = SHA256_EXPAND (w4_t,  wf_t, w7_t, w6_t);  \
    w7_t = SHA256_EXPAND (w5_t,  w0_t, w8_t, w7_t);  \
    w8_t = SHA256_EXPAND (w6_t,  w1_t, w9_t, w8_t);  \
    w9_t = SHA256_EXPAND (w7_t,  w2_t, wa_t, w9_t);  \
    wa_t = SHA256_EXPAND (w8_t,  w3_t, wb_t, wa_t);  \
    wb_t = SHA256_EXPAND (w9_t,  w4_t, wc_t, wb_t);  \
    wc_t = SHA256_EXPAND (wa_t,  w5_t, wd_t, wc_t);  \
    wd_t = SHA256_EXPAND (wb_t,  w6_t, we_t, wd_t);  \
    we_t = SHA256_EXPAND (wc_t,  w7_t, wf_t, we_t);  \
    wf_t = SHA256_EXPAND (wd_t,  w8_t, w0_t, wf_t);  \
  }

  #define ROUND_STEP(i)                                                                   \
  {                                                                                       \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w0_t, k_sha256[i +  0]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w1_t, k_sha256[i +  1]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w2_t, k_sha256[i +  2]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w3_t, k_sha256[i +  3]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w4_t, k_sha256[i +  4]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w5_t, k_sha256[i +  5]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w6_t, k_sha256[i +  6]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w7_t, k_sha256[i +  7]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w8_t, k_sha256[i +  8]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, w9_t, k_sha256[i +  9]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, wa_t, k_sha256[i + 10]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, wb_t, k_sha256[i + 11]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, wc_t, k_sha256[i + 12]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, wd_t, k_sha256[i + 13]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, we_t, k_sha256[i + 14]); \
    SHA256_STEP (MAJ, CH, a, b, c, d, e, f, g, h, T12, wf_t, k_sha256[i + 15]); \
  }

  ////
  // Unroll t=1..64 loop per 16
  ////
  ROUND_STEP (0);

  ROUND_EXPAND();
  ROUND_STEP(16);

  ROUND_EXPAND();
  ROUND_STEP(32);

  ROUND_EXPAND();
  ROUND_STEP(48);

  // compute i-th intermidiate value H(i)
  digest[0] += a;
  digest[1] += b;
  digest[2] += c;
  digest[3] += d;
  digest[4] += e;
  digest[5] += f;
  digest[6] += g;
  digest[7] += h;
} 

static void sha256(__global const unsigned int* pass, unsigned int pass_len, unsigned int* hash, const uint idx)
// static void sha256(__local const unsigned int *pass, int pass_len, unsigned int* hash)
{
    // printf("> pass_len=%d\n", pass_len);
    int num32Words = pass_len/4;
    if (mod(pass_len, 4)) // number of full 32-bit M-words
      num32Words++;
    printf("> [sha256:%d] num32Words=%d\n", idx, num32Words);

    // printf("> num32Words=%d\n", num32Words);
    
    // calculate padding bytes
    // int numMsgBlocks = 1;//pass_len/64 + 1;
    // (N msg-bytes + 1 + 64-bit Msg length)/64
    // +1: cause 1bit shall be placed, but the data
    //      discretization step is 1-byte (1-hex)
    int numMsgBlocks_DEBUG = (pass_len + 8)/64 + 1;

    // 56 B, cause 56 B = 55 B (Msg) + 1 B (1-splitter)
    // 52-55 B (msg) + 1 B (1-splitter)
    // if (mod(pass_len, 56) < 9) 
    //   numMsgBlocks_DEBUG++;// number of full 32-bit M-words

    int left3dWords_totalBlocks = numMsgBlocks_DEBUG * 16;


    // int num32WordsTotal = numMsgBlocks * 16;
    // int pad32Words = num32WordsTotal - num32Words;
    // printf("> pad32Words=%d\n", pad32Words);
    // printf("> num32WordsTotal=%d\n", num32WordsTotal);
    printf("> [DEBUG] numMsgBlocks_DEBUG =%d\n", numMsgBlocks_DEBUG);

    /*
    if <= 55-byte -> OK == add 1 + write Length to the end
    if < 64-byte -> add 1 + {write Length} to another MSG
    if <=64-byte -> {add 1 + write Length} to another MSG
    */


    //int msg_len_bytes = pass_len/4;
    int nleft_32Words = num32Words;
    // int nleft_bytes_pad = 

    unsigned int* p = hash;
    unsigned int W[0x10]={0};

    unsigned int State[8]={0};
    State[0] = 0x6a09e667;//+
    State[1] = 0xbb67ae85;//+
    State[2] = 0x3c6ef372;//+
    State[3] = 0xa54ff53a;//+
    State[4] = 0x510e527f;//+
    State[5] = 0x9b05688c;//+
    State[6] = 0x1f83d9ab;//+
    State[7] = 0x5be0cd19;//+

    printf(
      "> [sha256:%d] Hstart: p0=%08x p1=%08x p2=%08x p3=%08x p4=%08x p5=%08x p6=%08x p7=%08x\n",
      idx, State[0],State[1],State[2],State[3],State[4],State[5],State[6],State[7]
    );
    uint debug_loop_counter = 0;
    bool byteWasPlaced = false;
    bool placeByteForwardMsg = false;
    while (nleft_32Words > 0 || left3dWords_totalBlocks > 0)//|| !byteWasPlaced)// || pad32Words > 0)
    {
        left3dWords_totalBlocks-=16;
        printf("\n> nleft_32Words:%d, left3dWords_totalBlocks=%d\n", nleft_32Words, left3dWords_totalBlocks);
        W[0x0]=0x0;
        W[0x1]=0x0;
        W[0x2]=0x0;
        W[0x3]=0x0;
        W[0x4]=0x0;
        W[0x5]=0x0;
        W[0x6]=0x0;
        W[0x7]=0x0;
        W[0x8]=0x0;
        W[0x9]=0x0;
        W[0xA]=0x0;
        W[0xB]=0x0;
        W[0xC]=0x0;
        W[0xD]=0x0;
        W[0xE]=0x0;
        W[0xF]=0x0;
        printf("> [sha256:%d] BEFOREFORLOOP W[0]:%8x W[1]:%8x W[ 2]:%8x W[ 3]:%8x W[ 4]:%8x W[ 5]:%8x W[ 6]:%8x W[ 7]:%8x\n", idx, W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
        printf("> [sha256:%d] BEFOREFORLOOP W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n",idx, W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);
        

        // fill t=0..15 as M<t, i>
        // printf("> [Read msg] : START\n");
        for (int m=0; nleft_32Words > 0 && m<16 ;m++)
        {
            // printf("> %8x ");
            W[m]^=pass[num32Words - nleft_32Words];//SWAP(pass[num32Words - nleft_32Words]);
            printf("> W[%2d]=%8x, num32Words=%i, nleft_32Words=%i, msg_ind=%d\n", m, W[m], num32Words,nleft_32Words, num32Words - nleft_32Words);
            nleft_32Words--;
        }
        printf("> [sha256:%d]       FORLOOP W[0]:%8x W[1]:%8x W[ 2]:%8x W[ 3]:%8x W[ 4]:%8x W[ 5]:%8x W[ 6]:%8x W[ 7]:%8x\n", idx, W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
        printf("> [sha256:%d]       FORLOOP W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n",idx, W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);
        

        // printf("    W[0]:%8x W[1]:%8x W[2]:%8x W[3]:%8x W[4]:%8x W[5]:%8x W[6]:%8x W[7]:%8x\n", W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
        // printf("    W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n", W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);
        // printf("> [Read msg] : END, nleft_32Words=%d\n", nleft_32Words);

        // add 1 // && left3dWords_totalBlocks==0
        if (nleft_32Words < 1 && !byteWasPlaced)// && mod(pass_len, 64)!=0
        {
            uint v = mod( (num32Words-1), 16);// PRoblem if 48
            if (mod(pass_len, 4)==0)
              v++;
            if (v==16)
            {
              placeByteForwardMsg=true;
            } else {
              uint bit_shift = (pass_len - pass_len/4 * 4) * 8;//32 - (pass_len - pass_len/4 * 4) * 8;
              // printf("> bit shift=%d [B]\n", bit_shift/8 );
              ////// unsigned int padding = 0x80 << (((pass_len+4) - ((pass_len + 4)/4 * 4)) * 8);//?
              uint padding = 0x80000000 >> bit_shift;//?
              // uint padding = 0x200 >> bit_shift;//?

              printf("> [sha256:%d] bit_shift=%d, padding=%d, %i\n", idx, bit_shift, padding, (pass_len - pass_len/4 * 4) * 8 );
              // uint v = mod( num32Words, 16);
              printf("> [sha256:%d] bit shift=%08x in v=%i, num32Words=%d\n", idx, padding, v, num32Words);
              // printf("> [sha256:%d] bit swap =%8x in v=%i\n", idx, SWAP(padding), v );

              // printf("> Before pad:\n");
              // printf("    W[0]:%8x W[1]:%8x W[2]:%8x W[3]:%8x W[4]:%8x W[5]:%8x W[6]:%8x W[7]:%8x\n", W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
              // printf("    W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n", W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);

              // W[v/4] |= SWAP(padding);
              printf("> [sha256:%d] W[%i] =%08x\n", idx, v, W[v]);
              W[v] |= padding;//SWAP(padding);//padding;//


              // printf("> After pad:\n");
              // printf(">   W[0]:%8x W[1]:%8x W[2]:%8x W[3]:%8x W[4]:%8x W[5]:%8x W[6]:%8x W[7]:%8x\n", W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
              // printf(">   W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n", W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);
              byteWasPlaced = true;
            }
            if (placeByteForwardMsg)
            {
              // TODO: how to fill padded message if it's mod(, 64)==0?
              W[0x0] = 0x10101010;//0x80000000;
              W[0x1] = 0x10101010;// TODO: what to do with a standard??
              W[0x2] = 0x10101010;
              W[0x3] = 0x10101010;
              W[0x4] = 0x10101010;
              W[0x5] = 0x10101010;
              W[0x6] = 0x10101010;
              W[0x7] = 0x10101010;
              W[0x8] = 0x10101010;
              W[0x9] = 0x10101010;
              W[0xA] = 0x10101010;
              W[0xB] = 0x10101010;
              W[0xC] = 0x10101010;
              W[0xD] = 0x10101010;
              // W[0xE] = 0x10101010;
              // W[0xF] = 0x10101010;
            }

        }

        // printf("\n> [DEBUG] left3dWords_totalBlocks=%d\n", left3dWords_totalBlocks);
        if (left3dWords_totalBlocks<= 0)
        {
          W[0x0F] = pass_len * 8;
          // printf(">   W[0]:%8x W[1]:%8x W[2]:%8x W[3]:%8x W[4]:%8x W[5]:%8x W[6]:%8x W[7]:%8x\n", W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
          // printf(">   W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n", W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);
        }

        printf("> [sha256:%d] W[0]:%8x W[1]:%8x W[ 2]:%8x W[ 3]:%8x W[ 4]:%8x W[ 5]:%8x W[ 6]:%8x W[ 7]:%8x\n", idx, W[0x0], W[0x1], W[0x2], W[0x3], W[0x4], W[0x5], W[0x6], W[0x7]);
        printf("> [sha256:%d] W[8]:%8x W[9]:%8x W[10]:%8x W[11]:%8x W[12]:%8x W[13]:%8x W[14]:%8x W[15]:%8x\n",idx, W[0x8], W[0x9], W[0xA], W[0xB], W[0xC], W[0xD], W[0xE], W[0xF]);
        
        printf(
          "> [sha256:%d:Before] H: State0=%08x State1=%08x State2=%08x State3=%08x State4=%08x State5=%08x State6=%08x State7=%08x\n",
          idx, State[0],State[1],State[2],State[3],State[4],State[5],State[6],State[7]
        );

        // fill t=16..63 as M<t, i>
        sha256_process2(W, State);//state == h
        debug_loop_counter++;

        printf(
          "> [sha256:%d:After] H: State0=%08x State1=%08x State2=%08x State3=%08x State4=%08x State5=%08x State6=%08x State7=%08x\n",
          idx, State[0],State[1],State[2],State[3],State[4],State[5],State[6],State[7]
        );
    }
    printf("> [sha256:%d] loop end\n", idx);

    printf("> [sha256:%d] debug_loop_counter=%d\n", idx, debug_loop_counter);

    printf(
      "> [sha256:%d] H: p0=%08x p1=%08x p2=%08x p3=%08x p4=%08x p5=%08x p6=%08x p7=%08x\n",
      idx, State[0],State[1],State[2],State[3],State[4],State[5],State[6],State[7]
    );
    // p[0]=SWAP(State[0]);
    // p[1]=SWAP(State[1]);
    // p[2]=SWAP(State[2]);
    // p[3]=SWAP(State[3]);
    // p[4]=SWAP(State[4]);
    // p[5]=SWAP(State[5]);
    // p[6]=SWAP(State[6]);
    // p[7]=SWAP(State[7]);
    p[0]=State[0];
    p[1]=State[1];
    p[2]=State[2];
    p[3]=State[3];
    p[4]=State[4];
    p[5]=State[5];
    p[6]=State[6];
    p[7]=State[7];

    return;
}

typedef struct {
	unsigned int length;
	unsigned int buffer[N_BUFFER_sz];
} inbuf;

typedef struct {
  unsigned int buffer[8];
} outbuf;

__kernel void func_sha256(__global const inbuf * inbuffer, __global outbuf * outbuffer)
{
    unsigned int idx = get_global_id(0);
    unsigned int hash[8]={0};
    
    // copy to local memory
    // unsigned int length = inbuffer[idx].length;
    // __local unsigned int buffer[32/4];// = inbuffer[idx].buffer;
    // for (int i=0; i < length; i++)
    //   buffer[i] = inbuffer[idx].buffer[i];
    
    // for (int i=0; i < 32/4; i++)
    //   printf("inbuffer[idx].buffer[i]=%08x\n", inbuffer[idx].buffer[i]);
    
    // sha256(buffer, length, hash);
    printf("\n");
    sha256(inbuffer[idx].buffer, inbuffer[idx].length, hash, idx);
    // printf("H[%d]: H0=%08x H1=%08x H2=%08x H3=%08x H4=%08x H5=%08x H6=%08x H7=%08x\n\n\n", idx, hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7]);

    outbuffer[idx].buffer[0]=hash[0];
    outbuffer[idx].buffer[1]=hash[1];
    outbuffer[idx].buffer[2]=hash[2];
    outbuffer[idx].buffer[3]=hash[3];
    outbuffer[idx].buffer[4]=hash[4];
    outbuffer[idx].buffer[5]=hash[5];
    outbuffer[idx].buffer[6]=hash[6];
    outbuffer[idx].buffer[7]=hash[7];
    barrier(CLK_LOCAL_MEM_FENCE);
    // for (int i=0; i < 8; i++)
    //     printf("outbuf[%d].buffer[i]==%08x\n", idx, hash[i]);
}
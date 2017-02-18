/*
    姓名：李泽龙
    学号：14331144
    学院：数据科学与计算机学院
    方向：计应

    本程序的算法是使用了维基百科上，MD5算法的伪代码
    经测试，本程序可以对任意的string字符串进行散列计算

    然而，本程序有一个bug，我无法将其修正。
    就是对于同一个字符串，本程序产生的结果与维基百科产生的结果并不吻合。
    例如维基百科上，空文的散列为d41d8cd98f00b204e9800998ecf8427e
    而本程序产生的散列为117bbb7a513bf9f3b8f1413169b281c2

    不过，本程序的散列效果依然可以
    如对于“hello world”，产生的散列为05821cb9a65247f32b462b13fd338ac1
    而对于“hello worle”，产生的散列为f8695ac77e98f6aea7b3b7983bcbcb40
*/

#include <iostream>
#include <string.h>
#include <string>
#include <cmath>
#include <stdio.h>
using namespace std;

typedef unsigned int UINT32;

void MD5(string message)
{
    //Note: All variables are unsigned 32 bits and wrap modulo 2^32 when calculating
    //r specifies the per-round shift amounts
    UINT32 r[64] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };

    //Use binary integer part of the sines of integers as constants:
    UINT32 k[64];
    unsigned long long number = 1;
    for (int i = 0; i < 64; ++i)
    {
        k[i] = floor(abs(sin(i + 1)) * (number << 32));
    }

    //Initialize variables:
    UINT32 h0 = 0x67452301;
    UINT32 h1 = 0xEFCDAB89;
    UINT32 h2 = 0x98BADCFE;
    UINT32 h3 = 0x10325476;

    //Pre-processing:
    //append "1" bit to message
    UINT32 length = 8 * message.length();
    char append1 = 0x80;
    message += append1;

    //append "0" bits until message length in bits ≡ 448 (mod 512)
    int appendNumber = ((512 + 448 - length % 512)) % 512 / 8 - 1;
    char append0 = 0x0;
    for (int i = 0; i < appendNumber; ++i)
    {
        message += append0;
    }

    //append bit length of message as 64-bit little-endian integer to message
    char appendEnd;
    UINT32 length2 = length;
    for (int i = 0; i < 8; ++i)
    {
        appendEnd = length2 % 128;
        message += appendEnd;
        length2 = length2 / 128; 
    }

    //Process the message in successive 512-bit chunks:
    UINT32 chunkNumber = 8 * message.length() / 512;
    for (int j = 0; j < chunkNumber; ++j)
    {
        //break chunk into sixteen 32-bit little-endian words w[i], 0 ≤ i ≤ 15
        UINT32 w[16];
        UINT32 index;
        for (int i = 0; i < 16; ++i)
        {
            index = j * 64 + i * 4;
            w[i] = (char)message.at(index) |
                   (((char)message.at(index + 1)) << 8) |
                   (((char)message.at(index + 2)) << 16) |
                   (((char)message.at(index + 3)) << 24);
        }

        //Initialize hash value for this chunk:
        UINT32 a = h0;
        UINT32 b = h1;
        UINT32 c = h2;
        UINT32 d = h3;

        //Main loop:
        for (int i = 0; i < 64; ++i)
        {
            UINT32 f;
            UINT32 g;
            if (0 <= i && i <= 15)
            {
                f = (b & c) | ((~b) & d);
                g = i;
            }
            else if (16 <= i <= 31)
            {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            }
            else if (32 <= i <= 47)
            {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            }
            else if (48 <= i <= 63)
            {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }
            UINT32 temp = d;
            d = c;
            c = b;
            b = ( ((a + f + k[i] + w[g]) << r[i]) | ((a + f + k[i] + w[g]) >> (32 - r[i])) ) + b;
            a = temp;
        }
        //Add this chunk's hash to result so far:
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
    }
    //digest = h0 append h1 append h2 append h3 (expressed as little-endian)
    unsigned short digest[16];
    for (int i = 0; i < 4; ++i)
    {
        digest[i] = (h0 >> (24 - i * 8)) % (1 << 8);
    }
    for (int i = 4; i < 8; ++i)
    {
        digest[i] = (h1 >> (24 - (i - 4) * 8)) % (1 << 8);
    }
    for (int i = 8; i < 12; ++i)
    {
        digest[i] = (h2 >> (24 - (i - 8) * 8)) % (1 << 8);
    }
    for (int i = 12; i < 16; ++i)
    {
        digest[i] = (h3 >> (24 - (i - 12) * 8)) % (1 << 8);
    }
    for (int i = 0; i < 16; ++i)
    {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

int main()
{
    string input;
    while (getline(cin, input))
    {
        MD5(input);
    }
    return 0;
}
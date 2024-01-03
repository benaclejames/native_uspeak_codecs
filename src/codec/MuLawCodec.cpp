//
// Created by benaclejames on 1/2/24.
//

#include "MuLawCodec.h"
#include <climits>
#include <cstdlib>

MuLawCodec::MuLawCodec() {
    // Cache Encoding
    for (int i = SHRT_MIN; i <= SHRT_MAX; i++) {
        const int sign = ( i & 0x8000 ) >> 8;
        if( sign != 0 )
            i = -i;
        if( i > MAX ) i = MAX;
        i += BIAS;
        int exponent = 7;
        for( int expMask = 0x4000; ( i & expMask ) == 0; exponent--, expMask >>= 1 ) { }
        const int mantissa = ( i >> ( exponent + 3 ) ) & 0x0f;
        const char mulaw = static_cast<char>(sign | exponent << 4 | mantissa);
        pcmToMuLawMap[(i & 0xffff)] = static_cast<char>(~mulaw);
    }

    // Cache Decoding
    for (int i = 0; i < CHAR_MAX; i ++) {
        char mulaw = static_cast<char>(~i);
        int sign = mulaw & 0x80;
        int exponent = ( mulaw & 0x70 ) >> 4;
        int data = mulaw & 0x0f;
        data |= 0x10;
        data <<= 1;
        data += 1;
        data <<= exponent + 2;
        data -= BIAS;
        muLawToPcmMap[i] = static_cast<short>(sign == 0 ? data : -data);
    }
}

char MuLawCodec::MuLawEncode(short pcm) const {
    return pcmToMuLawMap[(pcm & 0xffff)];
}

char* MuLawCodec::Encode(short data[], int size, int mode) {
    char* encoded = new char[ size ];
    for (int i = 0; i < size; i++) {
        encoded[i] = MuLawEncode(data[i]);
    }
    return encoded;
}

short* MuLawCodec::Decode(char data[], int size, int mode) {
    short* decoded = new short[ size ];
    for( int i = 0; i < size; i++ )
    {
        decoded[ i ] = muLawToPcmMap[ data[ i ] ];
    }
    return decoded;
}

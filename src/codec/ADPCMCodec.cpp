//
// Created by benaclejames on 1/2/24.
//

#include "ADPCMCodec.h"

#include <climits>

ADPCMCodec::ADPCMCodec() {
    Init();
}

void ADPCMCodec::Init() {
    predictedSample = 0;
    stepsize = 7;
    index = 0;
    newSample = 0;
}

short ADPCMCodec::ADPCM_Decode(char originalSample) {
    int diff = 0;
    diff = ( stepsize * originalSample / 4 ) + ( stepsize / 8 );
    if( ( originalSample & 4 ) == 4 )
        diff += stepsize;
    if( ( originalSample & 2 ) == 2 )
        diff += stepsize >> 1;
    if( ( originalSample & 1 ) == 1 )
        diff += stepsize >> 2;
    diff += stepsize >> 3;

    if( ( originalSample & 8 ) == 8 )
        diff = -diff;

    newSample = diff;

    if( newSample > SHRT_MAX )
        newSample = SHRT_MAX;
    else if( newSample < SHRT_MIN )
        newSample = SHRT_MIN;

    index += indexTable[ originalSample ];
    if( index < 0 )
        index = 0;
    if( index > 88 )
        index = 88;

    stepsize = stepsizeTable[ index ];

    return static_cast<short>(newSample);
}

char ADPCMCodec::ADPCM_Encode(short originalSample) {
    int diff = ( originalSample - predictedSample );
    if( diff >= 0 )
        newSample = 0;
    else
    {
        newSample = 8;
        diff = -diff;
    }

    char mask = 4;
    int tempStepSize = stepsize;
    for( int i = 0; i < 3; i++ )
    {
        if( diff >= tempStepSize )
        {
            newSample |= mask;
            diff -= tempStepSize;
        }
        tempStepSize >>= 1;
        mask >>= 1;
    }

    //diff = 0;
    diff = stepsize >> 3;
    if( ( newSample & 4 ) != 0 )
        diff += stepsize;
    if( ( newSample & 2 ) != 0 )
        diff += stepsize >> 1;
    if( ( newSample & 1 ) != 0 )
        diff += stepsize >> 2;

    if( ( newSample & 8 ) != 0 )
        diff = -diff;

    predictedSample += diff;

    if( predictedSample > SHRT_MAX )
        predictedSample = SHRT_MAX;
    if( predictedSample < SHRT_MIN )
        predictedSample = SHRT_MIN;

    index += indexTable[ newSample ];
    if( index < 0 )
        index = 0;
    else if( index > 88 )
        index = 88;

    stepsize = stepsizeTable[ index ];

    return static_cast<char>(newSample);
}

char* ADPCMCodec::Encode(short data[], int size, int mode) {
    Init();
    int len = size / 2;
    if( len % 2 != 0 )
        len++;
    char* temp = new char[ len ];
    for( int i = 0; i < len; i++ )
    {
        if( ( i * 2 ) >= size )
            break;

        const char a = ADPCM_Encode( data[ i * 2 ] );
        char b = 0;
        if( ( ( i * 2 ) + 1 ) < size )
        {
            b = ADPCM_Encode( data[ ( i * 2 ) + 1 ] );
        }
        const char c = static_cast<char>((b << 4) | a);

        temp[ i ] = c;
    }
    return temp;
}

short* ADPCMCodec::Decode(char data[], int size, int mode) {
    Init();
    short* temp = new short[ size * 2 ];
    for( int i = 0; i < size; i++ )
    {
        const char c = data[ i ];
        const char d = static_cast<char>(c & 0x0f);
        const char e = static_cast<char>(c >> 4);
        temp[ i * 2 ] = ADPCM_Decode( d );
        temp[ ( i * 2 ) + 1 ] = ADPCM_Decode( e );
    }
    return temp;
}



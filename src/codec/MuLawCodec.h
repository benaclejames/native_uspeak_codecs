//
// Created by benaclejames on 1/2/24.
//

#ifndef MULAWCODEC_H
#define MULAWCODEC_H
#include "../codec.h"


class MuLawCodec : public Codec {
    char pcmToMuLawMap[65536]{};
    short muLawToPcmMap[256]{};
    const int BIAS = 0x84;
    const int MAX = 32635;

    MuLawCodec();
    char MuLawEncode(short pcm) const;
public:
    char* Encode(short data[], int size, int mode) override;
    short* Decode(char data[], int size, int mode) override;
};



#endif //MULAWCODEC_H

//
// Created by benaclejames on 1/2/24.
//

#ifndef CODEC_H
#define CODEC_H

class Codec {
public:
    virtual ~Codec() = default;

    virtual char* Encode(short data[], int size, int mode);
    virtual short* Decode(char data[], int size, int mode);
};

#endif //CODEC_H

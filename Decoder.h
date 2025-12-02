#ifndef CPU_DECODER_H
#define CPU_DECODER_H
#include <cstdint>
#include <iostream>

#include "CPU.h"

class Decoder {
public:
    Decoder() = default;
    void executar(uint32_t comando, CPU& cpu);
private:
    void IL(uint32_t comando, CPU& cpu);
    void S (uint32_t comando, CPU& cpu);
    void R(uint32_t comando, CPU& cpu);
    void I(uint32_t comando, CPU& cpu);
    void ULUI(uint32_t comando, CPU& cpu);
    void UAUIPC(uint32_t comando, CPU& cpu);
    void B(uint32_t comando, CPU& cpu);
    void J(uint32_t comando, CPU& cpu);
    void IJALR(uint32_t comando, CPU& cpu);
    void IFENCE(uint32_t comando, CPU& cpu);
    void IE(uint32_t comando, CPU& cpu);
};


#endif //CPU_DECODER_H
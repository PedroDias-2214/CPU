#ifndef CPU_BARRAMENTO_H
#define CPU_BARRAMENTO_H

#include <cstdint>
#include "Memoria.h"

class Barramento {
public:
    Barramento(Memoria* memoria);
    int32_t ler(uint32_t endereco) const;
    void escrever(uint32_t endereco, int32_t valor) const;

private:
    Memoria* memoria;
};


#endif //CPU_BARRAMENTO_H
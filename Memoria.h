//
// Created by pedro on 30/11/2025.
//

#ifndef CPU_MEMORIA_H
#define CPU_MEMORIA_H
#include <cstdint>
#include <vector>


class Memoria {
public:
    Memoria();

    static constexpr uint32_t TAMANHO_MEMORIA = 0x9FFFF + 1;
    int32_t ler(uint32_t endereco) const;
    void escrever(uint32_t endereco, int32_t valor);
    void carregar_programa(const std::vector<uint32_t>& programa);

private:
    std::vector<uint8_t> dados;
};


#endif //CPU_MEMORIA_H
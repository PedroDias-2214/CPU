#ifndef CPU_MMU_H
#define CPU_MMU_H

#include <cstdint>
#include <vector>

#include "Barramento.h"

// MMU com cache direta de 256 linhas, linha de 1 palavra (32 bits)
class MMU {
public:
    explicit MMU(Barramento* barramento_sistema);

    int32_t ler(uint32_t endereco);
    void escrever(uint32_t endereco, int32_t valor);

    // Estatísticas simples (poderiam ser usadas para o bônus de performance)
    [[nodiscard]] uint64_t get_hits() const { return hits; }
    [[nodiscard]] uint64_t get_misses() const { return misses; }

private:
    struct LinhaCache {
        bool     valido;
        uint32_t tag;
        int32_t  dado;
    };

    static constexpr uint32_t NUM_LINHAS = 256;

    Barramento* barramento;
    std::vector<LinhaCache> cache;

    uint64_t hits  = 0;
    uint64_t misses = 0;

    // Cálculo de índice/tag para cache mapeada diretamente.
    // Offset de palavra = 2 bits (endereços alinhados em 4 bytes),
    // índice = 8 bits (256 linhas), resto é tag.
    static uint32_t calcular_indice(uint32_t endereco) {
        return (endereco >> 2) & (NUM_LINHAS - 1); // bits [9:2]
    }

    static uint32_t calcular_tag(uint32_t endereco) {
        return endereco >> 10; // remove 2 (offset) + 8 (índice)
    }
};

#endif //CPU_MMU_H
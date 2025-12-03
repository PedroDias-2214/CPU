#include "MMU.h"

MMU::MMU(Barramento* barramento_sistema)
    : barramento(barramento_sistema),
      cache(NUM_LINHAS)
{
    for (auto &linha : cache) {
        linha.valido = false;
        linha.tag    = 0;
        linha.dado   = 0;
    }
}

int32_t MMU::ler(uint32_t const endereco) {
    uint32_t const indice = calcular_indice(endereco);
    uint32_t const tag    = calcular_tag(endereco);

    LinhaCache &linha = cache[indice];

    if (linha.valido && linha.tag == tag) {
        // HIT de cache
        ++hits;
        return linha.dado;
    }

    // MISS de cache: busca na memória principal via barramento
    ++misses;
    int32_t const valor = barramento->ler(endereco);

    linha.valido = true;
    linha.tag    = tag;
    linha.dado   = valor;

    return valor;
}

void MMU::escrever(uint32_t const endereco, int32_t const valor) {
    uint32_t const indice = calcular_indice(endereco);
    uint32_t const tag    = calcular_tag(endereco);

    LinhaCache &linha = cache[indice];

    // Política simples: write-through + write-allocate
    // Se a linha correspondente estiver em cache e com mesmo tag, atualiza também a cache.
    if (linha.valido && linha.tag == tag) {
        linha.dado = valor;
    }

    // Sempre escreve na memória principal via barramento
    barramento->escrever(endereco, valor);
}
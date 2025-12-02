#include "Memoria.h"

#include <iostream>
#include <stdexcept>

Memoria::Memoria() {
    this->dados.resize(TAMANHO_MEMORIA, 0); // Estende a memória ao tamanho dela e zera tudo
}

int32_t Memoria::ler(uint32_t const endereco) const {
    if (endereco+3 >= TAMANHO_MEMORIA) {
        throw std::out_of_range("ERRO: Tentativa de leitura inválida, fora dos limites da memória!");
    }

    uint8_t const b0 = dados[endereco];
    uint8_t const b1 = dados[endereco+1];
    uint8_t const b2 = dados[endereco+2];
    uint8_t const b3 = dados[endereco+3];

    return (b3 << 24 | b2 << 16 | b1 << 8 | b0);
}

void Memoria::escrever(uint32_t const endereco, int32_t const valor) {
    if (endereco+3 >= TAMANHO_MEMORIA) {
        throw std::out_of_range("ERRO: Tentativa de leitura inválida, fora dos limites da memória!");
    }
    dados[endereco] = valor & 0xFF;
    dados[endereco+1] = (valor >> 8) & 0xFF;
    dados[endereco+2] = (valor >> 16) & 0xFF;
    dados[endereco+3] = (valor >> 24) & 0xFF;

    if (endereco >= 0x80000 && endereco <= 0x8FFFF) { // ALTERAÇÃO NA VRAM
        std::cout << "[Update na VRAM] Endereco 0x" << std::hex << endereco <<
            " Valor: " << std::dec << valor << std::endl;
    }
}

void Memoria::carregar_programa(const std::vector<uint32_t> &programa) {
    for (size_t i = 0; i < programa.size(); i++) {
        escrever(i * 4, programa[i]);
    }
}

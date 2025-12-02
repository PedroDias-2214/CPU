//
// Created by pedro on 30/11/2025.
//

#include "Barramento.h"

Barramento::Barramento(Memoria *memoria) {
    this->memoria = memoria;
}

int32_t Barramento::ler(uint32_t const endereco) const {
    return this->memoria->ler(endereco);
}

void Barramento::escrever(uint32_t const endereco, int32_t const valor) const {
    this->memoria->escrever(endereco, valor);
}

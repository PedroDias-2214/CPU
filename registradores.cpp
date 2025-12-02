#include "registradores.h"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <vector>

registradores::registradores(){
    this->iniciar_registradores();
}

void registradores::iniciar_registradores() {
    this->registrador.resize(32);
    this->registrador[2] = 0x7FFFF; // Registrador 2 deve apontar para o topo da memoria disponivel
}

void registradores::mostrar_memorias() const {
    std::cout << "MOSTRANDO AS MEMORIAS EM NUMEROS DECIMAIS!" << std::endl;
    for (int i=0; i<32; i++) {
        std::cout << 'x' << std::setw(2) << std::setfill(' ') << i << ' ' << registrador[i] << std::endl;
    }
    std::cout << "FIM DAS MEMORIAS!" << std::endl;
}

void registradores::modificar_memoria(int const numero_reg, const int valor) { // se vier mais de 5 bits, ele ignora
    if (numero_reg == 0 || numero_reg >31) return; // Isso impede de mexer no registrador 0
    // e em registradores fora do escopo
    registrador[numero_reg] = valor;
    std::cout << "Registrador X" << numero_reg << " modificado!" << std::endl;
    std::cout << "Valor: " << valor << std::endl;
}

int registradores::get_valor_registrador(int const numero_reg) const {
    if (numero_reg < 0 || numero_reg > 31) throw std::logic_error(std::string("Registrador " +
        std::to_string(numero_reg) + " não existe"));
    return registrador[numero_reg];
}

void registradores::mostrar_memorias_binario() const {
    std::cout << "MOSTRANDO MEMORIAS EM BINARIO!" << std::endl;
    for (int i=0; i<32; i++) {
        std::bitset<32> bits (this->registrador[i]);
        std::cout << 'x' << std::setw(2) << std::setfill(' ') << i << ": " << bits << std::endl;
    }
    std::cout << "FIM DAS MEMORIAS!" << std::endl;
}
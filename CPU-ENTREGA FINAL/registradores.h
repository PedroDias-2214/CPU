//
// Created by pedro on 20/11/2025.
//

#ifndef CPU_REGISTRADOR_H
#define CPU_REGISTRADOR_H
#include <vector>

class registradores {
public:
    registradores();
    void mostrar_memorias() const;
    void mostrar_memorias_binario() const;
    void modificar_memoria(int numero_reg, int valor);
    [[nodiscard]] int get_valor_registrador(int numero_reg) const;

private:
    void iniciar_registradores();
    std::vector<int> registrador;
};

#endif //CPU_REGISTRADOR_H
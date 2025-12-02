#include <iostream>
#include <iomanip> // Mostrar código HEX

#include "CPU.h"
#include "Decoder.h"
#include "Memoria.h"
#include "MMU.h"

int main () {

    CPU cpu;
    Decoder decoder;
    Memoria memoria;
    Barramento barramento(&memoria);
    MMU mmu(&barramento);
    cpu.mmu = &mmu;

    // https://venus.kvakil.me/

    std::vector<uint32_t> const soma10_20 = { // programa para teste 1
        0xfe010113, // addi x2 x2 -32
        0x00112e23, // sw x1 28(x2)
        0x00812c23, // sw x8 24(x2)
        0x02010413, // addi x8 x2 32
        0x00a00513, // addi x10 x0 10
        0xfea42a23, // sw x10 -12(x8)
        0x01400513, // addi x10 x0 20
        0xfea42823, // sw x10 -16(x8)
        0xff442503, // lw x10 -12(x8)
        0xff042583, // lw x11 -16(x8)
        0x00b50533, // add x10 x10 x11
        0xfea42623, // sw x10 -20(x8)
        0xfec42503, // lw x10 -20(x8)
        0x01c12083, // lw x1 28(x2)
        0x01812403, // lw x8 24(x2)
        0x02010113, // addi x2 x2 32
        0x0 // Encerrar Programa
        //0x00a00093, // ADDI, X1, X0, 10
        //0x01400113, // ADDI, X2, X0, 20
        //0x002081b3,  // ADD, X3, X1, X2
    };
    std::vector<uint32_t> const busca_binaria = {
        0x20000493, // addi x9 x0 512 - 512 é uma posição fixa na memória!
        // o código deu alguns problemas já que a memória ram vai até certo número,
        // porém a posição da memória que os sites tentavam acessar não existia
        0x00a00913, // addi x18 x0 10
        0x02800993, // addi x19 x0 40 - Esse 40 é o número que estamos procurando no vetor
        // se quiser mudar esse comando, só ir em https://venus.kvakil.me/ e escrever
        // "addi x19 x0 valor", os valores existentes são 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
        // Qualquer outro valor deverá retornar -1
        // valores de 10 a 100:
        // 10 - 0x00a00993
        // 20 - 0x01400993
        // 30 - 0x01e00993
        // 40 - 0x02800993
        // 50 - 0x03200993
        // 60 - 0x03c00993
        // 70 - 0x04600993
        // 80 - 0x05000993
        // 90 - 0x05a00993
        // 100- 0x06400993
        // 200 (para retornar -1) - 0x0c800993
        0x00000293, // addi x5 x0 0
        0xfff90313, // addi x6 x18 -1
        0xfff00513, // addi x10 x0 -1
        0x02534e63, // blt x6 x5 60
        0x405303b3, // sub x7 x6 x5
        0x4013d393, // srai x7 x7 1
        0x007283b3, // add x7 x5 x7
        0x00239e13, // slli x28 x7 2
        0x01c48e33, // add x28 x9 x28
        0x000e2e83, // lw x29 0(x28)
        0x013e8c63, // beq x29 x19 24
        0x013ec663, // blt x29 x19 12
        0xfff38313, // addi x6 x7 -1
        0xfd9ff06f, // jal x0 -40
        0x00138293, // addi x5 x7 1
        0xfd1ff06f, // jal x0 -48
        0x00038513, // addi x10 x7 0
        0x0040006f, // jal x0 4
        0x0 // encerrar programa
    };

    std::vector<uint32_t> const memoria_instrucoes = { // mesma memoria de soma10_20

        0xfe010113,
        0x00112e23,
        0x00812c23,
        0x02010413,
        0x00a00513,
        0xfea42a23,
        0x01400513,
        0xfea42823,
        0xff442503,
        0xff042583,
        0x00b50533,
        0xfea42623,
        0xfec42503,
        0x01c12083,
        0x01812403,
        0x02010113,

        //0x00a00093, // ADDI, X1, X0, 10
        //0x01400113, // ADDI, X2, X0, 20
        //0x002081b3, // ADD, X3, X1, X2
        0x0 // Encerrar Programa
    };
    // Programa de tratamento de interrupcao (handler) em RISC-V:
    // - addi x10, x10, 1   (0x00150513)
    // - ret (jalr x0, x1, 0) (0x00008067)
    // - 0 para encerrar
    std::vector<uint32_t> const handler_interrupcao = {
        0x00150513,
        0x00008067,
        0x0
    };

    constexpr uint32_t ENDERECO_HANDLER = 0x00001000; // Endereco fixo do handler na RAM

    // Carrega o handler na RAM no endereco fixo
    for (size_t i = 0; i < handler_interrupcao.size(); ++i) {
        memoria.escrever(ENDERECO_HANDLER + static_cast<uint32_t>(i) * 4, handler_interrupcao[i]);
    }

    std::cout << "=== Simulador RISC-V com Cache e Interrupcao ===" << std::endl;
    std::cout << "1 - Rodar soma 10 + 20" << std::endl;
    std::cout << "2 - Rodar busca binaria" << std::endl;
    std::cout << "3 - Editar memoria (endereco/valor) antes de executar" << std::endl;
    std::cout << "Opcao: ";

    int opcao = 0;
    std::cin >> opcao;

    if (opcao == 3) {
        while (true) {
            std::cout << "[Edicao de memoria] Informe endereco em decimal (-1 para sair): ";
            int addr;
            std::cin >> addr;
            if (addr < 0) break;
            std::cout << "Valor (int32): ";
            int val;
            std::cin >> val;
            memoria.escrever(static_cast<uint32_t>(addr), val);
        }
        std::cout << "Escolha agora o programa (1-soma, 2-busca): ";
        std::cin >> opcao;
    }

    if (opcao == 1) {
        memoria.carregar_programa(soma10_20);
    } else {
        memoria.carregar_programa(busca_binaria);
    }

    int endereco_base = 512;
    const std::vector<int> dados_vetor = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    for (int i = 0; i < static_cast<int>(dados_vetor.size()); i++) {
        // Escreve 10, 20, 30... na memória
        // Cada inteiro ocupa 4 bytes, por isso i*4
        memoria.escrever(endereco_base + (i * 4), dados_vetor[i]);
    }


    // Loop principal de execucao com pipeline simples de 2 estagios (IF/ID-EX)
    char modo;
    std::cout << "Executar passo-a-passo? (s/n): ";
    std::cin >> modo;
    bool passo_a_passo = (modo == 's' || modo == 'S');

    struct EstagioPipeline {
        bool valido;
        uint32_t pc;
        uint32_t instrucao;
    };

    EstagioPipeline decode { false, 0, 0 };
    uint32_t pc_fetch = 0;
    bool parar = false;

    while (true) {
        bool fez_pulo_ciclo = false; // controla desvios neste ciclo do pipeline

        // Estagio IF (busca) - busca proxima instrucao
        EstagioPipeline proximo_decode { false, 0, 0 };
        if (!parar) {
            proximo_decode.pc = pc_fetch;
            proximo_decode.instrucao = mmu.ler(pc_fetch);
            proximo_decode.valido = true;
        }

        // Estagio ID/EX (decodificacao/execucao) - executa instrucao anterior
        if (decode.valido) {
            cpu.PC = decode.pc;

            std::cout << "[PIPE] PC: " << decode.pc << std::endl;
            std::cout << "[PIPE] Instrucao: 0x" << std::hex <<
                std::setw(8) << std::setfill('0') << decode.instrucao << std::dec << std::endl;

            if (decode.instrucao == 0) {
                std::cout << "Programa encerrado, 0x0 executado!" << std::endl;
                parar = true;
            } else {
                try {
                    cpu.fez_pulo = false;
                    decoder.executar(decode.instrucao, cpu);
                } catch (std::exception const & e) {
                    std::cerr << "ERRO: " << e.what() << std::endl;
                    parar = true;
                }

                if (!cpu.fez_pulo) {
                    cpu.PC += 4;
                } else {
                    std::cout << "Fez pulo para " << cpu.PC << std::endl;
                    fez_pulo_ciclo = true;
                }

                // Se estavamos em interrupcao e retornamos para o PC salvo, sair do modo de interrupcao
                if (cpu.em_interrupcao && cpu.PC == cpu.pc_retorno_interrupcao) {
                    cpu.em_interrupcao = false;
                    std::cout << "Retorno da rotina de interrupcao para PC = " << cpu.PC << std::endl;
                }
            }

            std::cout << std::endl;

            if (passo_a_passo && !parar) {
                std::cout << "Pressione 'c' e ENTER para continuar, 'i' para interrupcao, 'q' para sair: ";
                char c;
                std::cin >> c;

                if (c == 'q' || c == 'Q') {
                    std::cout << "Execucao interrompida pelo usuario." << std::endl;
                    break;
                }

                if (!cpu.em_interrupcao && (c == 'i' || c == 'I')) {
                    std::cout << "Interrupcao externa detectada (tecla 'i')" << std::endl;
                    cpu.em_interrupcao = true;
                    cpu.pc_retorno_interrupcao = cpu.PC + 4; // proxima instrucao
                    cpu.regs.modificar_memoria(1, static_cast<int>(cpu.pc_retorno_interrupcao)); // x1 = endereco retorno
                    cpu.PC = ENDERECO_HANDLER;
                    fez_pulo_ciclo = true;
                }
            }
        }

        if (parar) break;

        // Atualizacao do PC de fetch e do estagio decode (controle de desvios no pipeline)
        if (fez_pulo_ciclo) {
            // Em caso de salto, proxima busca vem do novo PC e descartamos instrucao buscada neste ciclo
            pc_fetch = cpu.PC;
            decode.valido = false;
        } else {
            pc_fetch += 4;
            decode = proximo_decode;
        }
    }

    cpu.regs.mostrar_memorias();

    std::cout << "REGISTRADOR DE RETORNO x10: " << cpu.regs.get_valor_registrador(10) << std::endl;
    std::cout << "Cache hits: " << mmu.get_hits() << " | misses: " << mmu.get_misses() << std::endl;

    return 0;
}
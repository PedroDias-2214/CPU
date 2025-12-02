//
// Created by pedro on 22/11/2025.
//

#include "Decoder.h"
#include "CPU.h"

enum class OPCODES : uint8_t { // CLASSE CRIADA PARA SIMPLIFICAÇÃO E SUPORTE APENAS
    R = 0b0110011,
    I = 0b0010011,
    ULUI = 0b0110111,
    UAUIPC = 0b0010111,
    B = 0b1100011,
    J = 0b1101111,
    IJALR = 0b1100111,
    IFENCE = 0b0001111,
    IE = 0b1110011,
    IL = 0b0000011,
    S = 0b0100011
};

void Decoder::executar(uint32_t const comando, CPU &cpu) {
    switch (uint8_t OPCODE = comando & 0x7F) {
        case static_cast<uint8_t>(OPCODES::R):      R(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::I):      I(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::ULUI):   ULUI(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::UAUIPC): UAUIPC(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::B):      B(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::J):      J(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::IJALR):  IJALR(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::IFENCE): IFENCE(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::IE):     UAUIPC(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::IL):     IL(comando, cpu); break;
        case static_cast<uint8_t>(OPCODES::S):      S(comando, cpu); break;

        default: throw std::logic_error("OPCODE não implementado!");
    }
}


void Decoder::R(const uint32_t comando, CPU& cpu) {
    uint8_t const rd =     (comando >> 7) & 0b11111;
    uint8_t const funct3 = (comando >> 12) & 0b111;
    uint8_t const rs1 =    (comando >> 15) & 0b11111;
    uint8_t const rs2 =    (comando >> 20) & 0b11111;
    uint8_t const funct7 = (comando >> 25) & 0b1111111;
    // SLL , SRL , SRA , ADD , SUB , XOR , OR , AND , SLT , SLTU

    switch (funct3) {
        case 0b000: // ADD e SUB
            if (funct7 == 0b0000000) cpu.ADD(rd, rs1, rs2);
            else if (funct7 == 0b0100000) cpu.SUB(rd, rs1, rs2);
            else throw std::logic_error("funct7 não pertence ao mesmo funct3");
            break;

            // IMPORTANTE: a partir daqui, apenas o funct7 do srl e sra importa, então será
            // descartado para as outras funções
        case 0b001: // SLL
            cpu.SLL(rd, rs1, rs2);
            break;

        case 0b010: // SLT
            cpu.SLT(rd, rs1, rs2);
            break;

        case 0b011: // SLTU
            cpu.SLTU(rd, rs1, rs2);
            break;

        case 0b100: // XOR
            cpu.XOR(rd, rs1, rs2);
            break;

        case 0b101: // SRA e SRL
            if (funct7 == 0b0100000) cpu.SRA(rd, rs1, rs2);
            else cpu.SRL(rd, rs1, rs2);
            break;

        case 0b110: // OR
            cpu.OR(rd, rs1, rs2);
            break;

        case 0b111:
            cpu.AND(rd, rs1, rs2);
            break;

        default:
            throw std::logic_error("Comando não encontrado!");
    }
}

void Decoder::I(const uint32_t comando, CPU& cpu) {
    // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI

    const uint8_t rd =     (comando >> 7) & 0b11111;
    const uint8_t funct3 = (comando >> 12) & 0b111;
    const uint8_t rs1 =    (comando >> 15) & 0b11111;

    if (funct3 == 0b001 || funct3 == 0b101) { // divisão diferente, utiliza shamt
    // SLLI, SRLI, SRAI
        const uint8_t shamt = (comando >> 20) & 0b11111;
        const uint8_t funct7 = (comando >> 25) & 0b1111111;

        switch (funct3) {
            case 0b001: // SLLI
                cpu.SLLI(rd, rs1, shamt);
                break;
            default: // sempre que cair aqui é sempre 0b101 (SRAI, SRLI)
                if (funct7 == 0b0100000) cpu.SRAI(rd, rs1, shamt); // SRAI
                else cpu.SRLI(rd, rs1, shamt); // SRLI
                break;
        }
    }

    else { // ADDI, SLTI, SLTIU, XORI, ORI, ANDI
        uint32_t imm = comando >> 20;

        if (imm & 0x800) {
            imm |= 0xFFFFF000;
        }
        switch (funct3) {
            case 0b000: // ADDI
                cpu.ADDI(rd, rs1, imm);
                break;

            case 0b010: // SLTI
                cpu.SLTI(rd, rs1, imm);
                break;

            case 0b011: // SLTIU
                cpu.SLTIU(rd, rs1, imm);
                break;

            case 0b100: // XORI
                cpu.XORI(rd, rs1, imm);
                break;

            case 0b110: // ORI
                cpu.ORI(rd, rs1, imm);
                break;

            case 0b111: // ANDI
                cpu.ANDI(rd, rs1, imm);
                break;

            default:
                throw std::logic_error("Funct3 desconhecido!");
        }
    }
}

void Decoder::ULUI(const uint32_t comando, CPU& cpu) {
    uint8_t const rd =   (comando >> 7) & 0b11111;
    uint32_t const imm = (comando >> 12) & 0b11111111111111111111;
    cpu.LUI(rd, imm);
}

void Decoder::UAUIPC(const uint32_t comando, CPU& cpu) { // mesma separação do ULUI, mas faz outra coisa
    uint8_t const rd =   (comando >> 7) & 0b11111;
    uint32_t const imm = (comando >> 12) & 0b11111111111111111111;
    cpu.AUIPC(rd, imm);
}

void Decoder::B(const uint32_t comando, CPU& cpu) {
    // BEQ, BNE, BLT, BGE, BLTU, BGEU
    const uint8_t funct3 = (comando >> 12) & 0x7;
    uint8_t rs1 = (comando >> 15) & 0x1F;
    uint8_t rs2 = (comando >> 20) & 0x1F;

    // Reconstrução do Imediato Tipo B (Scrambled)
    const int32_t imm12   = (comando >> 31) & 0x1;    // Bit 31 -> pos 12
    const int32_t imm10_5 = (comando >> 25) & 0x3F;   // Bits 30:25 -> pos 10:5
    const int32_t imm4_1  = (comando >> 8) & 0xF;     // Bits 11:8 -> pos 4:1
    const int32_t imm11   = (comando >> 7) & 0x1;     // Bit 7 -> pos 11

    // O bit 0 é sempre 0 em branches
    int32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);

    // Extensão de sinal (Bit 12 é o sinal)
    if (imm & 0x1000) {
        imm |= 0xFFFFE000;
    }

    switch (funct3) {
        case 0b000: // BEQ
            cpu.BEQ(rs1, rs2, imm);
            break;

        case 0b001: // BNE
            cpu.BNE(rs1, rs2, imm); // todo: IMPLEMENTAR BNE NA CPU
            break;

        case 0b100: // BLT
            cpu.BLT(rs1, rs2, imm); // todo: IMPLEMENTAR BLT NA CPU
            break;

        case 0b101: // BGE
            cpu.BGE(rs1, rs2, imm); // todo: IMPLEMENTAR BGE NA CPU
            break;

        case 0b110: // BLTU
            cpu.BLTU(rs1, rs2, imm); // todo: IMPLEMENTAR BLTU NA CPU
            break;

        case 0b111: // BGEU
            cpu.BGEU(rs1, rs2, imm); // todo: IMPLEMENTAR BGEU NA CPU
            break;

        default:
            throw std::logic_error("Funct3 desconhecido!");
    }
}

void Decoder::J(const uint32_t comando, CPU& cpu) {
    uint8_t const rd = (comando >> 7) & 0x1F;

    int32_t const imm20    = (comando >> 31) & 0b1; // Bit de sinal
    int32_t const imm10_1  = (comando >> 21) & 0x3FF;
    int32_t const imm11    = (comando >> 20) & 0b1;
    int32_t const imm19_12 = (comando >> 12) & 0xFF;

    int32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);

    if (imm20) {
        imm |= 0xFFE00000; // Preenche os bits 31 até 21 com 1s
    }

    cpu.JAL(rd, imm);
}

void Decoder::IJALR(const uint32_t comando, CPU& cpu) {
    uint8_t rd =  (comando >> 7) & 0b11111;
    uint8_t rs1 = (comando >> 15) & 0b11111;
    int32_t imm = (comando >> 20) & 0b111111111111;

    if (imm & 0x800) imm |= 0xFFFF0000;

    cpu.JALR(rd, rs1, imm);
}

void Decoder::IFENCE(const uint32_t comando, CPU& cpu) {
    return;
    // O CÓDIGO ABAIXO É PARA FAZER A SEPARAÇÃO, MAS COMO OS MÉTODOS NÃO SERÃO IMPLEMENTADOS,
    // É POSSÍVEL SÓ RETORNAR
    if (comando >> 7 != 0b00000 || comando >> 15 != 0b00000 || comando >> 27 != 0b0000) {
        throw std::logic_error ("Bits não zerados!");
    }
    if (comando >> 12 == 0b001) { // fence.i
        if (comando >> 20 != 0b00000000) throw std::logic_error ("Bits não zerados!");
        std::cout << "I fence.i" << std::endl;
    }
    else if (comando >> 12 == 0b000) { // fence
        uint8_t succ = (comando >> 20) & 0b1111;
        uint8_t pred = (comando >> 24) & 0b1111;
    }
}

void Decoder::IE(const uint32_t comando, CPU& cpu) {
    return;
    // O CÓDIGO ABAIXO É PARA FAZER A SEPARAÇÃO, MAS COMO OS MÉTODOS NÃO SERÃO IMPLEMENTADOS,
    // É POSSÍVEL SÓ RETORNAR
    if (comando >> 7 != 0b0000000000000) {
        throw std::logic_error("Bits não zerados!"); // se os bits não estiverem zerados, pode ser q o opcode fosse ser outro
    }

    if (comando >> 20 == 0b000000000000) {
        std::cout << "ecall" << std::endl;
    }
    else if (comando >> 20 == 0b00000000001) { // é um if para verificar erros, o código não assume que será uma entrada correta
        std::cout << "ebreak" << std::endl;
    }

    else throw std::logic_error("Comando inválido"); // se cair aqui, não é 000(...)1 nem 000(...)0, então não existe o comando
}

void Decoder::IL(uint32_t const comando, CPU& cpu) {
    uint8_t const rd = (comando >> 7) & 0b11111;
    uint8_t const funct3 = (comando >> 12) & 0b111;
    uint8_t const rs1 = (comando >> 15) & 0b11111;
    int32_t imm = (comando >> 20); // Pega 12 bits

    if (imm & 0x800) imm |= 0xFFFFF000;

    if (funct3 == 0b010) { // LW (só esse é utilizado no método)
        cpu.LW(rd, rs1, imm);
    }
}

void Decoder::S(uint32_t const comando, CPU& cpu) {
    uint8_t const imm4_0 = (comando >> 7) & 0b11111;
    uint8_t const funct3 = (comando >> 12) & 0b111;
    uint8_t const rs1 = (comando >> 15) & 0b11111;
    uint8_t const rs2 = (comando >> 20) & 0b11111;
    uint8_t const imm11_5 = (comando >> 25) & 0b1111111;

    int32_t imm = (imm11_5 << 5) | imm4_0;
    if (imm & 0x800) imm |= 0xFFFFF000;
    if (funct3 == 0b010) { // SW (só esse é utilizado no método)
        cpu.SW(rs1, rs2, imm);
    }
}
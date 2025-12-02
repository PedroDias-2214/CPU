#include "CPU.h"

CPU::CPU(){
    this->regs = registradores();
    this->PC = 0;
    this->fez_pulo = false;
    this->em_interrupcao = false;
    this->pc_retorno_interrupcao = 0;
    this->mmu = nullptr;
}

void CPU::ADD(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) +
        this->regs.get_valor_registrador(rs2));
}

void CPU::ADDI(uint8_t const rd, uint8_t const rs1, int const imm) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) + imm);
}

void CPU::SUB(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) -
        this->regs.get_valor_registrador(rs2));
}

void CPU::AND(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) &
        this->regs.get_valor_registrador(rs2));
}

void CPU::ANDI(uint8_t const rd, uint8_t const rs1, int const imm) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1)
        & imm);
}

void CPU::OR(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) |
        this->regs.get_valor_registrador(rs2));
}

void CPU::ORI(uint8_t const rd, uint8_t const rs1, int const imm) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) | imm);
}

void CPU::XOR(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) ^
        this->regs.get_valor_registrador(rs2));
}

void CPU::XORI(uint8_t const rd, uint8_t const rs1, int const imm) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) ^ imm);
}

void CPU::SLL(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    int const quantidade = this->regs.get_valor_registrador(rs2) & 0b11111;
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) <<
        quantidade);
}

void CPU::SLLI(uint8_t const rd, uint8_t const rs1, int const shamt) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) <<
        shamt);
}

void CPU::SRL(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    int const quantidade = this->regs.get_valor_registrador(rs2) & 0b11111;
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) >>
        quantidade);
}

void CPU::SRLI(uint8_t const rd, uint8_t const rs1, int const shamt) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) >>
        shamt);
}

void CPU::SRA(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    int const shamt = this->regs.get_valor_registrador(rs2) & 0b11111;
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) >> shamt);
}

void CPU::SRAI(uint8_t const rd, uint8_t const rs1, int const shamt) {
    this->regs.modificar_memoria(rd, this->regs.get_valor_registrador(rs1) >> shamt);
}

void CPU::LUI(uint8_t const rd, int const imm) {

    this->regs.modificar_memoria(rd, imm << 12);
}

void CPU::AUIPC(uint8_t const rd, int const imm) {
    this->regs.modificar_memoria(rd, this->PC + (imm << 12));
}

void CPU::SLT(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    int const val1 = this->regs.get_valor_registrador(rs1);
    int const val2 = this->regs.get_valor_registrador(rs2);
    // Comparação com sinal (int padrão)
    this->regs.modificar_memoria(rd, (val1 < val2) ? 1 : 0);
}

void CPU::SLTI(uint8_t const rd, uint8_t const rs1, int const imm) {
    int const val1 = this->regs.get_valor_registrador(rs1);
    this->regs.modificar_memoria(rd, (val1 < imm) ? 1 : 0);
}

void CPU::SLTU(uint8_t const rd, uint8_t const rs1, uint8_t const rs2) {
    uint32_t const val1 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs1));
    uint32_t const val2 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs2));
    this->regs.modificar_memoria(rd, (val1 < val2) ? 1 : 0);
}

void CPU::SLTIU(uint8_t const rd, uint8_t const rs1, int const imm) {
    uint32_t const val1 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs1));
    uint32_t const imm_u = static_cast<uint32_t>(imm);
    this->regs.modificar_memoria(rd, (val1 < imm_u) ? 1 : 0);
    // função ternária, se val1 for menor que imm_u, retorna 1, se não, retorna 0
}

void CPU::BEQ(uint8_t const rs1, uint8_t const rs2, int const imm) {
    if (this->regs.get_valor_registrador(rs1) == this->regs.get_valor_registrador(rs2)) {
        this->PC += imm;
        this->fez_pulo = true;
    }
}

void CPU::BNE(uint8_t const rs1, uint8_t const rs2, int const imm) {
    if (this->regs.get_valor_registrador(rs1) != this->regs.get_valor_registrador(rs2)) {
        this->PC += imm;
        this->fez_pulo = true;
    }
}

void CPU::BLT(uint8_t const rs1, uint8_t const rs2, int const imm) {
    if (this->regs.get_valor_registrador(rs1) < this->regs.get_valor_registrador(rs2)) {
        this->PC += imm;
        this->fez_pulo = true;
    }
}

void CPU::BGE(uint8_t const rs1, uint8_t const rs2, int const imm) {
    if (this->regs.get_valor_registrador(rs1) >= this->regs.get_valor_registrador(rs2)) {
        this->PC += imm;
        this->fez_pulo = true;
    }
}

void CPU::BLTU(uint8_t const rs1, uint8_t const rs2, int const imm) {
    uint32_t const val1 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs1));
    uint32_t const val2 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs2));
    if (val1 < val2) {
        this->PC += imm;
        this->fez_pulo = true;
    }
}

void CPU::BGEU(uint8_t const rs1, uint8_t const rs2, int const imm) {
    uint32_t const val1 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs1));
    uint32_t const val2 = static_cast<uint32_t>(this->regs.get_valor_registrador(rs2));
    if (val1 >= val2) {
        this->PC += imm;
        this->fez_pulo = true;
    }
}

void CPU::JAL(uint8_t const rd, int const imm) {
    this->regs.modificar_memoria(rd, this->PC + 4);
    this->PC += imm;
    this->fez_pulo = true;
}

void CPU::JALR(uint8_t const rd, uint8_t const rs1, int const imm) {
    this->regs.modificar_memoria(rd, this->PC + 4);

    int const rs1_val = this->regs.get_valor_registrador(rs1);
    int alvo = rs1_val + imm;

    alvo &= ~1;

    this->PC = alvo;
    this->fez_pulo = true;
}

void CPU::FENCE() {} // essas não são utilizadas
void CPU::FENCEI() {}
void CPU::ECALL() {}
void CPU::EBREAK() {}

void CPU::LW(uint8_t const rd, const uint8_t rs1, int const imm) {
    int const endereco = this->regs.get_valor_registrador(rs1) + imm;
    int32_t const valor = this->mmu->ler(endereco);
    this->regs.modificar_memoria(rd, valor);
}

void CPU::SW(uint8_t const rs1, uint8_t const rs2, int const imm) const {
    int const endereco =  this->regs.get_valor_registrador(rs1) + imm;
    int32_t const valor = this->regs.get_valor_registrador(rs2);
    this->mmu->escrever(endereco, valor);
}
#ifndef CPU_H
#define CPU_H
#include <iostream>
#include <cstdint>

#include "registradores.h"
#include "MMU.h"

class CPU {
public:
    bool fez_pulo;
    bool em_interrupcao;
    uint32_t pc_retorno_interrupcao;
    registradores regs;
    MMU *mmu;
    uint32_t PC;

    CPU();
    void SLL(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void SLLI(uint8_t rd, uint8_t rs1, int shamt); // feito
    void SRL(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void SRLI(uint8_t rd, uint8_t rs1, int shamt); // feito
    void SRA(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void SRAI(uint8_t rd, uint8_t rs1, int shamt); // feito
    void ADD(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void ADDI(uint8_t rd, uint8_t rs1, int imm); // feito
    void SUB(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void LUI(uint8_t rd, int imm);
    void AUIPC(uint8_t rd, int imm);
    void XOR(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void XORI(uint8_t rd, uint8_t rs1, int imm); // feito
    void OR(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void ORI(uint8_t rd, uint8_t rs1, int imm); // feito
    void AND(uint8_t rd, uint8_t rs1, uint8_t rs2); // feito
    void ANDI(uint8_t rd, uint8_t rs1, int imm); // feito
    void SLT(uint8_t rd, uint8_t rs1, uint8_t rs2);
    void SLTI(uint8_t rd, uint8_t rs1, int imm);
    void SLTU(uint8_t rd, uint8_t rs1, uint8_t rs2);
    void SLTIU(uint8_t rd, uint8_t rs1, int imm);
    void BEQ(uint8_t rs1, uint8_t rs2, int imm);
    void BNE(uint8_t rs1, uint8_t rs2, int imm);
    void BLT(uint8_t rs1, uint8_t rs2, int imm);
    void BGE(uint8_t rs1, uint8_t rs2, int imm);
    void BLTU(uint8_t rs1, uint8_t rs2, int imm);
    void BGEU(uint8_t rs1, uint8_t rs2, int imm);
    void JAL(uint8_t rd, int imm);
    void JALR(uint8_t rd, uint8_t rs1, int imm);
    void FENCE();
    void FENCEI();
    void ECALL();
    void EBREAK();

    void LW(uint8_t rd, uint8_t rs1, int imm);
    void SW(uint8_t rs1, uint8_t rs2, int imm) const;
};

#endif
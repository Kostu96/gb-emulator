#include "cpu.h"
#include "memory_map.h"

void CPU::IMP()
{
}

void CPU::IMM()
{
    m_operandAddress = PC++;
}

void CPU::IMMEX()
{
    // TODO: this can be merged with IMM using is16bit
    m_operandAddress = PC;
    PC += 2;
}

void CPU::REG()
{}

void CPU::INR()
{
    m_operandAddress = HL;
}

void CPU::ABS()
{}

void CPU::MZP()
{}

void CPU::REL()
{}

void CPU::MIDX()
{}

void CPU::BITA()
{}

void CPU::NOP()
{}

void CPU::LD()
{
    uint8_t operand = 0;
    uint16_t operand16 = 0;

    if (!m_isStore)
        operand = (this->*m_readByteFunc)(m_operandAddress);

    if (m_is16bit) {
        if (!m_isStore) {
            operand16 = (this->*m_readByteFunc)(m_operandAddress + 1);
            operand16 <<= 8;
            operand16 |= operand;
        }

        switch (m_currentOpcode) {
        case 0x21:
            HL = operand16;
            break;
        case 0x31:
            SP = operand16;
            break;
        case 0x32:
            m_memoryMap.store8(m_operandAddress, A);
        }
    }
}

void CPU::INC()
{}

void CPU::DEC()
{}

void CPU::RLCA()
{}

void CPU::ADD()
{}

void CPU::RRCA()
{}

void CPU::STOP()
{}

void CPU::RLA()
{}

void CPU::JR()
{}

void CPU::RRA()
{}

void CPU::DAA()
{}

void CPU::CPL()
{}

void CPU::SCF()
{}

void CPU::CCF()
{}

void CPU::HALT()
{}

void CPU::ADC()
{}

void CPU::SUB()
{}

void CPU::SBC()
{}

void CPU::AND()
{}

void CPU::XOR()
{
    if (m_isREG) {
        switch (m_currentOpcode) {
        case 0xAF:
            A ^= A;
            break;
        }
    }

    F.byte = 0;
    F.zeroFlag = A == 0;
}

void CPU::OR()
{}

void CPU::CP()
{}

void CPU::RET()
{}

void CPU::POP()
{}

void CPU::JP()
{}

void CPU::CALL()
{}

void CPU::PUSH()
{}

void CPU::RST()
{}

void CPU::CB()
{}

void CPU::RETI()
{}

void CPU::LDH()
{}

void CPU::DI()
{}

void CPU::EI()
{}

void CPU::RLC()
{}

void CPU::RRC()
{}

void CPU::RL()
{}

void CPU::RR()
{}

void CPU::SLA()
{}

void CPU::SRA()
{}

void CPU::SWAP()
{}

void CPU::SRL()
{}

void CPU::BIT()
{}

void CPU::RES()
{}

void CPU::SET()
{}

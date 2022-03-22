#include "cpu.h"
#include "memory_map.h"

void CPU::IMP()
{
}

void CPU::IMM()
{
    m_operandAddress = m_registerNamed.PC++;
}

void CPU::IMMEX()
{
    // TODO: this can be merged with IMM using is16bit
    m_operandAddress = m_registerNamed.PC;
    m_registerNamed.PC += 2;
}

void CPU::REG()
{}

void CPU::INR()
{
    m_operandAddress = m_registerNamed.HL;
}

void CPU::ABS()
{}

void CPU::MZP()
{}

void CPU::REL()
{}

void CPU::MIDX()
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
            m_registerNamed.HL = operand16;
            break;
        case 0x31:
            m_registerNamed.SP = operand16;
            break;
        case 0x32:
            m_memoryMap.store8(m_operandAddress, m_registerNamed.A);
            --m_registerNamed.HL;
            break;
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
            m_registerNamed.A ^= m_registerNamed.A;
            break;
        }
    }

    m_registerNamed.F.byte = 0;
    m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
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
{
    if (m_isCBInstruction) {
        uint8_t bit0 = (m_currentOpcode >> 0) & 1;
        uint8_t bit1 = (m_currentOpcode >> 1) & 1;
        uint8_t bit2 = (m_currentOpcode >> 2) & 1;
        uint8_t bit3 = (m_currentOpcode >> 2) & 1;
        uint8_t regIndex =
            ((!bit2 & bit1) | (bit2 & !bit1) | (bit2 & !bit0)) << 0 |
            ((bit2 & !bit0) | bit1) << 1 |
            ((bit2 & bit1) | !bit0) << 2;

        uint8_t targetBit = ((m_currentOpcode & 0x70) >> 4) | bit3;
        
            switch (m_currentOpcode) {
        case 0x7C:
            BIT(regIndex, targetBit);
            break;
        }
        m_isCBInstruction = false;
    }
    else
        m_isCBInstruction = true;
}

void CPU::RETI()
{}

void CPU::LDH()
{}

void CPU::DI()
{}

void CPU::EI()
{}

void CPU::RLC(uint8_t regIndex)
{}

void CPU::RRC(uint8_t regIndex)
{}

void CPU::RL(uint8_t regIndex)
{}

void CPU::RR(uint8_t regIndex)
{}

void CPU::SLA(uint8_t regIndex)
{}

void CPU::SRA(uint8_t regIndex)
{}

void CPU::SWAP(uint8_t regIndex)
{}

void CPU::SRL(uint8_t regIndex)
{}

void CPU::BIT(uint8_t regIndex, uint8_t targetBit)
{
    if ((m_currentOpcode & 0xF) == 0x6 || (m_currentOpcode & 0xF) == 0xE) {
        m_registerNamed.F.zeroFlag = ((this->*m_readByteFunc)(m_registerNamed.HL) >> targetBit) & 1;
    }
    else {
        m_registerNamed.F.zeroFlag = (m_registerArray[regIndex] >> targetBit) & 1;
    }
    
    m_registerNamed.F.subtractFlag = 0;
    m_registerNamed.F.halfCarryFlag = 1;
}

void CPU::RES(uint8_t regIndex)
{}

void CPU::SET(uint8_t regIndex)
{}

#include "cpu.h"
#include "memory_map.h"

void CPU::IMP() {}

void CPU::IMM()
{
    m_operandAddress = m_registerNamed.PC;
    m_registerNamed.PC += m_is16bit ? 2 : 1;
}

void CPU::INR()
{
    m_operandAddress = m_registerNamed.HL;
    switch (m_currentOpcode) {
    case 0x0A:
    case 0x02: m_operandAddress = m_registerNamed.BC; break;
    case 0x1A:
    case 0x12: m_operandAddress = m_registerNamed.DE; break;
    }
}

void CPU::ABS()
{
    m_operandAddress = (this->*m_readByteFunc)(m_registerNamed.PC + 1);
    m_operandAddress <<= 8;
    m_operandAddress |= (this->*m_readByteFunc)(m_registerNamed.PC);
    m_registerNamed.PC += 2;
}

void CPU::MZP()
{

}

void CPU::REL()
{
    m_operandAddress = m_registerNamed.PC++;
}

void CPU::MIDX()
{
    m_operandAddress = 0xFF00 | m_registerNamed.C;
}

void CPU::NOP() {}

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
        case 0x01: m_registerNamed.BC = operand16; break;
        case 0x11: m_registerNamed.DE = operand16; break;
        case 0x21: m_registerNamed.HL = operand16; break;
        case 0x31: m_registerNamed.SP = operand16; break;
        }
    }
    else {
        switch (m_currentOpcode) {
        case 0x0E:
            m_registerNamed.C = operand;
            break;
        case 0x32:
            storeByte(m_operandAddress, m_registerNamed.A);
            --m_registerNamed.HL;
            break;
        case 0x3E:
            m_registerNamed.A = operand;
            break;
        case 0x77:
        case 0xE2:
            storeByte(m_operandAddress, m_registerNamed.A);
            break;
        }
    }
}

void CPU::INC()
{
    // TODO: test halfcarry flag
    uint8_t bit4;
    switch (m_currentOpcode) {
    case 0x0C:
        bit4 = m_registerNamed.C & 0x10;
        ++m_registerNamed.C;
        m_registerNamed.F.halfCarryFlag = ((m_registerNamed.C & 0x10) ^ bit4) >> 4;
        m_registerNamed.F.subtractFlag = 0;
        m_registerNamed.F.zeroFlag = m_registerNamed.C == 0;
        break;
    case 0x03:
        bit4 = m_registerNamed.BC & 0x10;
        ++m_registerNamed.BC;
        m_registerNamed.F.halfCarryFlag = ((m_registerNamed.BC & 0x10) ^ bit4) >> 4;
        m_registerNamed.F.subtractFlag = 0;
        m_registerNamed.F.zeroFlag = m_registerNamed.BC == 0;
        break;
    case 0x13:
        bit4 = m_registerNamed.DE & 0x10;
        ++m_registerNamed.DE;
        m_registerNamed.F.halfCarryFlag = ((m_registerNamed.DE & 0x10) ^ bit4) >> 4;
        m_registerNamed.F.subtractFlag = 0;
        m_registerNamed.F.zeroFlag = m_registerNamed.DE == 0;
        break;
    case 0x23:
        bit4 = m_registerNamed.HL & 0x10;
        ++m_registerNamed.HL;
        m_registerNamed.F.halfCarryFlag = ((m_registerNamed.HL & 0x10) ^ bit4) >> 4;
        m_registerNamed.F.subtractFlag = 0;
        m_registerNamed.F.zeroFlag = m_registerNamed.HL == 0;
        break;
    case 0x33:
        bit4 = m_registerNamed.SP & 0x10;
        ++m_registerNamed.SP;
        m_registerNamed.F.halfCarryFlag = ((m_registerNamed.SP & 0x10) ^ bit4) >> 4;
        m_registerNamed.F.subtractFlag = 0;
        m_registerNamed.F.zeroFlag = m_registerNamed.SP == 0;
        break;
    }
}

void CPU::DEC()
{
    // TODO: test halfcarry flag
    uint8_t bit4;
    switch (m_currentOpcode) {
    case 0x05:
        bit4 = m_registerNamed.B & 0x10;
        --m_registerNamed.B;
        m_registerNamed.F.halfCarryFlag = ((m_registerNamed.B & 0x10) ^ bit4) >> 4;
        m_registerNamed.F.subtractFlag = 1;
        m_registerNamed.F.zeroFlag = m_registerNamed.B == 0;
        break;
    }
}

void CPU::RLCA()
{

}

void CPU::ADD()
{

}

void CPU::RRCA()
{

}

void CPU::STOP()
{

}

void CPU::RLA()
{
    uint8_t bit7 = (m_registerNamed.A >> 7) & 1;
    m_registerNamed.A <<= 1;
    m_registerNamed.A |= m_registerNamed.F.carryFlag;
    m_registerNamed.F.carryFlag = bit7;
}

void CPU::JR()
{
    int8_t rel = (this->*m_readByteFunc)(m_operandAddress);
    switch (m_currentOpcode) {
    case 0x20:
        if (m_registerNamed.F.zeroFlag == 1) {
            m_currentInstructionCyclesLeft += 4;
            m_registerNamed.PC += rel;
        }
        break;
    }
}

void CPU::RRA()
{

}

void CPU::DAA()
{

}

void CPU::CPL()
{

}

void CPU::SCF()
{

}

void CPU::CCF()
{

}

void CPU::HALT()
{

}

void CPU::ADC()
{

}

void CPU::SUB()
{

}

void CPU::SBC()
{

}

void CPU::AND()
{

}

void CPU::XOR()
{
    switch (m_currentOpcode) {
    case 0xAF:
        m_registerNamed.A ^= m_registerNamed.A;
        break;
    }

    m_registerNamed.F.byte = 0;
    m_registerNamed.F.zeroFlag = m_registerNamed.A == 0;
}

void CPU::OR()
{

}

void CPU::CP()
{

}

void CPU::RET()
{
    switch (m_currentOpcode) {
    case 0xC9:
        m_registerNamed.PC = (this->*m_readByteFunc)(m_registerNamed.SP + 1);
        m_registerNamed.PC <<= 8;
        m_registerNamed.PC |= (this->*m_readByteFunc)(m_registerNamed.SP);
        m_registerNamed.SP += 2;
        break;
    }
}

void CPU::POP()
{
    switch (m_currentOpcode) {
    case 0xC1:
        m_registerNamed.C = (this->*m_readByteFunc)(m_registerNamed.SP);
        m_registerNamed.B = (this->*m_readByteFunc)(m_registerNamed.SP + 1);
        break;
    case 0xD1:
        m_registerNamed.E = (this->*m_readByteFunc)(m_registerNamed.SP);
        m_registerNamed.D = (this->*m_readByteFunc)(m_registerNamed.SP + 1);
        break;
    case 0xE1:
        m_registerNamed.L = (this->*m_readByteFunc)(m_registerNamed.SP);
        m_registerNamed.H = (this->*m_readByteFunc)(m_registerNamed.SP + 1);
        break;
    case 0xF1:
        m_registerNamed.F.byte = (this->*m_readByteFunc)(m_registerNamed.SP);
        m_registerNamed.A = (this->*m_readByteFunc)(m_registerNamed.SP + 1);
        break;
    }
    m_registerNamed.SP += 2;
}

void CPU::JP()
{

}

void CPU::CALL()
{
    switch (m_currentOpcode) {
    case 0xCD:
        m_registerNamed.SP -= 2;
        storeByte(m_registerNamed.SP, m_registerNamed.PC & 0xFF);
        storeByte(m_registerNamed.SP + 1, (m_registerNamed.PC >> 8) & 0xFF);
        m_registerNamed.PC = m_operandAddress;
        break;
    }
}

void CPU::PUSH()
{
    m_registerNamed.SP -= 2;
    switch (m_currentOpcode) {
    case 0xC5:
        storeByte(m_registerNamed.SP, m_registerNamed.C);
        storeByte(m_registerNamed.SP + 1, m_registerNamed.B);
        break;
    case 0xD5:
        storeByte(m_registerNamed.SP, m_registerNamed.E);
        storeByte(m_registerNamed.SP + 1, m_registerNamed.D);
        break;
    case 0xE5:
        storeByte(m_registerNamed.SP, m_registerNamed.L);
        storeByte(m_registerNamed.SP + 1, m_registerNamed.H);
        break;
    case 0xF5:
        storeByte(m_registerNamed.SP, m_registerNamed.F.byte);
        storeByte(m_registerNamed.SP + 1, m_registerNamed.A);
        break;
    }
}

void CPU::RST()
{

}

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
{

}

void CPU::LDH()
{
    uint16_t address = 0xFF00 | (this->*m_readByteFunc)(m_operandAddress);
    if (m_isStore) {
        storeByte(address, m_registerNamed.A);
    }
    else {
        m_registerNamed.A = (this->*m_readByteFunc)(address);
    }
}

void CPU::DI()
{

}

void CPU::EI()
{

}

void CPU::RLC(uint8_t regIndex)
{

}

void CPU::RRC(uint8_t regIndex)
{

}

void CPU::RL(uint8_t regIndex)
{

}

void CPU::RR(uint8_t regIndex)
{

}

void CPU::SLA(uint8_t regIndex)
{

}

void CPU::SRA(uint8_t regIndex)
{

}

void CPU::SWAP(uint8_t regIndex)
{

}

void CPU::SRL(uint8_t regIndex)
{

}

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
{

}

void CPU::SET(uint8_t regIndex)
{

}

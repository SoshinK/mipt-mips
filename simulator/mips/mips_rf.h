/*
 * rf.h - mips register file
 * @author Pavel Kryukov pavel.kryukov@phystech.edu
 * Copyright 2015 MIPT-MIPS
 */

#ifndef RF_H
#define RF_H

#include <array>

#include "mips_instr.h"

class MIPSRF
{
        struct Reg {
            uint32 value = 0;
        };
        std::array<Reg, MIPSRegister::MAX_REG> array = {};

        Reg& get_entry( MIPSRegister num) { return array.at( num.to_size_t()); }
        const Reg& get_entry( MIPSRegister num) const { return array.at( num.to_size_t()); }

        uint32 read( MIPSRegister num) const
        {
            assert( !num.is_mips_hi_lo());
            return get_entry( num).value;
        }

        void write( MIPSRegister num, uint64 val)
        {
            if ( num.is_zero())
                return;
            if ( num.is_mips_hi_lo()) {
                write( MIPSRegister::mips_hi, val >> 32);
                write( MIPSRegister::mips_lo, val);
                return;
            }            

            auto& entry = get_entry(num);
            entry.value = val;
        }

    public:
        MIPSRF() = default;

        inline void read_source( MIPSInstr* instr, std::size_t src_index) const
        {
            if ( src_index == 0)
                instr->set_v_src1( read( instr->get_src1_num()));
            else
                instr->set_v_src2( read( instr->get_src2_num()));
        }

        inline void read_sources( MIPSInstr* instr) const
        {
            read_source( instr, 0);
            read_source( instr, 1);
        }

        inline void write_dst( const MIPSInstr& instr)
        {
            MIPSRegister reg_num  = instr.get_dst_num();
            bool writes_dst = instr.get_writes_dst();
            if ( !reg_num.is_zero() && writes_dst)
                write( reg_num, instr.get_v_dst());
            else
                write( reg_num, read(reg_num));
        }
};

#endif


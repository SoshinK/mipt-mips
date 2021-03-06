/*
 * func_sim.cpp - extremely simple simulator
 * @author Pavel Kryukov pavel.kryukov@phystech.edu
 * Copyright 2015 MIPT-MIPS
 */

#ifndef FUNC_SIM_H
#define FUNC_SIM_H

#include <memory>
#include <string>

#include <simulator.h>

#include "rf.h"

template <typename ISA>
class FuncSim : public Simulator
{
    using FuncInstr = typename ISA::FuncInstr;
    using Memory = typename ISA::Memory;
    
    private:
        std::unique_ptr<RF<ISA>> rf;
        Addr PC = NO_VAL32;
        Memory* mem = nullptr;

        uint64 nops_in_a_row = 0;
        void update_nop_counter( const FuncInstr& instr);

    public:
        explicit FuncSim( bool log = false);
        ~FuncSim() final;

        // Rule of five
        FuncSim( const FuncSim&) = delete;
        FuncSim( FuncSim&&) = delete;
        FuncSim operator=( const FuncSim&) = delete;
        FuncSim operator=( FuncSim&&) = delete;

        void init( const std::string& tr);
        FuncInstr step();
        void run(const std::string& tr, uint64 instrs_to_run) final;
        void set_PC(Addr value) final { PC = value; }
};

#endif

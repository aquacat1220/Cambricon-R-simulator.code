#ifndef DRAM_CNTRL
#define DRAM_CNTRL

//#include "../ext/dramsim3/src/configuration.h"
#include "dramsim3.h"
#include <functional>
#include <string>
#include <list>
#include <map>

#include "CambRCommon.hpp"

class dram_cntrler_base_t {
    public:
        dram_cntrler_base_t(const string& config_file, const string& output_dir) 
        : clk_(0) {
            mem_ = dramsim3::GetMemorySystem(config_file, output_dir,
                            bind(&dram_cntrler_base_t::read_callback, this, placeholders::_1),
                            bind(&dram_cntrler_base_t::write_callback, this, placeholders::_1));
        }

        virtual void read_callback(uint64_t addr) = 0;
        virtual void write_callback(uint64_t addr) = 0;
        void print_stats() { mem_->PrintStats(); }

    protected:
        dramsim3::MemorySystem *mem_;
        uint64_t clk_;
};

class timing_dram_cntrler_t : public dram_cntrler_base_t {
public: 
    timing_dram_cntrler_t(const string& config_file, const string& output_dir)
    : dram_cntrler_base_t(config_file, output_dir), 
      elapsed_time_(0), next_tick_time_(0) {}

    void read_callback(uint64_t addr);
    void write_callback(uint64_t addr);

    list<dram_message_t> to_dram_bus;
    list<dram_message_t> from_dram_bus;

    map<uint64_t, float> dram;

private:
    list<dram_message_t> wait_list_;
    
    double elapsed_time_;
    double next_tick_time_;
};

#endif
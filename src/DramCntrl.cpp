#include "DramCntrl.hpp"

void timing_dram_cntrler_t :: read_callback(uint64_t addr) {
    for (list<dram_message_t>::iterator iter = wait_list_.begin(); iter != wait_list_.end();) {
        if (iter->address == addr && !iter->is_write) {
            for (int off = 0; off < 64; off += 2) {
                if (dram.find(addr + off) != dram.end())
                    (*iter).data[off / 2] = dram[addr + off];
                else
                    (*iter).data[off / 2] = 0;
            }
            from_dram_bus.push_back(*iter);
            iter = wait_list_.erase(iter);
        } else
            iter++;
    }
}

void timing_dram_cntrler_t :: write_callback(uint64_t addr) {
    for (list<dram_message_t>::iterator iter = wait_list_.begin(); iter != wait_list_.end();) {
        if (iter->address == addr && iter->is_write) {
            for (int off = 0; off < 64; off += 2)
                dram[addr + off] = (*iter).data[off / 2];
            iter = wait_list_.erase(iter);
        } else
            iter++;
    }
}

void timing_dram_cntrler_t :: Cycle() {
    mem_->ClockTick();

    while (!to_dram_bus.empty()) {
        auto elem = to_dram_bus.front();
        if (!mem_->WillAcceptTransaction(elem.address, elem.is_write))
            break;

        mem_->AddTransaction(elem.address, elem.is_write);
        wait_list_.push_back(elem);
        to_dram_bus.pop_front();
    }

}

bool timing_dram_cntrler_t :: is_idle() {
    return wait_list_.empty() && to_dram_bus.empty() && from_dram_bus.empty();
}
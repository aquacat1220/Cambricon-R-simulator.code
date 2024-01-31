#ifndef AIBA_ARC
#define AIBA_ARC

#include "AIBAPP.hpp"
#include "AIBANode.hpp"

class AIBA {
    private:
        AIBAPP AIBAPP_;
        vector<AIBANode> AIBANodes_;
        vector<Sum> output_buffer_;

    public:
        AIBA();
        void Cycle(void);
        void Transition(void);
};

#endif
#ifndef AIBA_ARC
#define AIBA_ARC

#include "AIBAPP.hpp"
#include "AIBANode.hpp"

class AIBA{
    private:

    protected:
        AIBAPP _AIBAPP;
        vector<AIBANode> _AIBANodes;
        vector<Sum> _output_buffer;

    public:
        AIBA();
        void Cycle(void);
        void Transition(void);
};
#endif
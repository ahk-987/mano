#include <cstdint>
#include "../header/mymemory.hpp"
class simulator{
 
    uint16_t IR=0;
    uint16_t AC =0;
    uint16_t TR = 0;//no DR as did nt felt it will be required TR is temparory register
    uint16_t AR =0;
    uint16_t PC =0;
    memory ram ;
    bool E=false;
    bool hlt=0;
    public :
    simulator()
    {
        ram=memory();
    }
    simulator(uint16_t orginLoc,memory ram_module)
    {
        PC=orginLoc;
        ram=ram_module;
    }
        auto run(){
            while(!hlt){
                AR=PC++;
                IR=ram[AR];
            }
        }

        auto AND(uint16_t Adr,bool indirect)
        {
            AC&=indirect?ram[ram[Adr]]:ram[Adr];
        }
        
        auto ADD(uint16_t Adr,bool indirect)
        {
            TR=AC;
            AC+=indirect?ram[ram[Adr]]:ram[Adr];
            E=TR>AC?1:0;
        }
        auto LDA(uint16_t Adr,bool indirect)
        {
            AC=indirect?ram[ram[Adr]]:ram[Adr];
        }
        auto STA(uint16_t Adr,bool indirect)
        {
            indirect?ram[ram[Adr]]:ram[Adr]=AC;
        }
        auto BUN(uint16_t Adr,bool indirect)
        {
            PC=indirect?ram[ram[Adr]]:ram[Adr];
        }
        auto BSA(uint16_t Adr,bool indirect)
        {
            ram[Adr]=PC;//store PC to Adr
            BUN(Adr+1,indirect);//bun to adr+1
        }
        auto ISZ(uint16_t Adr,bool indirect)
        {
            PC+=(++(indirect?ram[ram[Adr]]:ram[Adr]))==0?1:0;
        }

};

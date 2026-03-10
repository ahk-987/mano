#include "../header/mymemory.hpp"
#include <print>
#include <cstdint>
class simulator{
 
    uint16_t IR=0;
    uint16_t AC =0;
    uint16_t TR = 0;//no DR as did nt felt it will be required TR is temparory register
    uint16_t AR =0;
    uint16_t PC =0;
    uint8_t command=0;//to store instruction
    memory ram ;
    bool E=false;
    bool hlt=0;
    public :
    simulator()
    {
        ram=memory();
    }
    simulator(uint16_t orginLoc,memory &ram_module)
    {
        PC=orginLoc;
        ram=ram_module;
        std::println("pc{}",PC);
    }
    
        inline  auto AND(uint16_t Adr,bool indirect)
        {
            AC&=indirect?ram[ram[Adr]]:ram[Adr];
        }
        
        inline  auto ADD(uint16_t Adr,bool indirect)
        {
            TR=AC;
            AC+=(indirect?ram[ram[Adr]]:ram[Adr]);
            E=TR>AC?1:0;
        }
        inline  auto LDA(uint16_t Adr,bool indirect)
        {
            AC=indirect?ram[ram[Adr]]:ram[Adr];
        }
        inline  auto STA(uint16_t Adr,bool indirect)
        {
            if(indirect)
            {ram[ram[Adr]]=AC;}
            else ram[Adr]=AC;
        }
        inline  auto BUN(uint16_t Adr,bool indirect)
        {
            PC=indirect?ram[ram[Adr]]:ram[Adr];
        }
        auto BSA(uint16_t Adr,bool indirect)
        {
            ram[Adr]=PC;//store PC to Adr
            BUN(Adr+1,indirect);//bun to adr+1
        }
        inline  auto ISZ(uint16_t Adr,bool indirect)
        {
            PC+=(++(indirect?ram[ram[Adr]]:ram[Adr]))==0?1:0;
        }
        auto register_reference(uint16_t Instruction)
        {
            switch(IR)
            {
                case 0x7800://CLA
                    AC=0;
                    break;
                case 0x7400://CLE
                    E=0;
                    break;
                case 0x7200://CMA
                    AC=(~AC);
                    break;
                case 0x7100://CME
                    E=(~E);
                    break;
                case 0x7080://CIR
                    AC=(AC>>1)+(E?0x8000:0);
                    break;
                case 0x7040://CIL
                    AC=(AC<<1)+E;
                    break;
                case 0x7020://INC
                    TR=AC;
                    AC++;
                    E=TR>AC?1:0;//if TR>AC only happens when overflow so it means 
                    break;      //it must propogate to E
                case 0x7010://SPA
                    TR=AC&0x8000;//the highest bit is sign bit only it is stored in TR
                    PC+=(TR==0)?1:0;//pc++ only when positive
                    break;
                case 0x7008://SNA
                    TR=AC&0x8000;
                    PC+=(TR==0)?0:1;//pc++ only when negative
                    break;
                case 0x7004://SZA
                    PC+=(AC==0)?1:0;
                    break;
                case 0x7002://SZE
                    PC+=(E==0)?1:0;
                    break;
                case 0x7001://HLT
                    hlt=1;
                    std::println("hlt called");
                    break;
                default:
                std::println("Why are You here !! :this is default of regref");
                
            }
        }
        auto memory_reference(uint8_t command,uint16_t AR,bool indirect)
        {

                switch (command&0x7) {//leaves out 4th bit the indirect bit  
                    case 0:
                        AND(AR,indirect);
                        break;
                    case 1:
                        ADD(AR,indirect);
                        break;
                    case 2:
                        LDA(AR,indirect);
                        break;
                    case 3: 
                        STA(AR,indirect);
                        break;
                    case 4:
                        BUN(AR,indirect);
                        break;
                    case 5:
                        BSA(AR,indirect);
                        break;
                    case 6:
                        ISZ(AR,indirect);
                        break;
                    default:
                        std::println("Why are You here ? memref default");
                }
    
        }
        auto run(){
            bool indirect;
            while(!hlt){
                AR=PC++;
                IR=ram[AR];
                indirect=IR>>15;//should provide us msb only (indirect check)
                AR=IR&0x0FFF;//only have bottom 12bits of address :)
                command=static_cast<uint8_t>(IR>>12);//to get top 4 bits
                if(command==0x7)
                {
                    register_reference(IR);
                    continue;
                }
                if(command==0xF)
                {
                    std::println("Sorry no support for IO Commands yet :(");
                    continue;
                }
                else{
                        memory_reference(command, AR, indirect);
                }
            }
        }
    };
    
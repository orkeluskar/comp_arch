#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include<sstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


// Group Members:
// 1. Omkar Keluskar [NetID : ork216]
// 2. Rushabh Patel [NetID: rgp296]

// Implemented instructions given in table of lab1.pdf
// Verified input for given test case in "imem.text" which results in appropriate values in "RFresult.txt" & "dmem.result.txt"

class RF
{
    public:
      

        bitset<32> ReadData1, ReadData2;
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);


        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];


            //for write operations
            if(WrtEnable.to_ulong() == 1){
            	Registers[WrtReg.to_ulong()] = WrtData;
            }

         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   

                 // implement the ALU operations by you.
                 if (ALUOP.to_ulong() == ADDU)
                  {
                  	ALUresult = bitset<32> (oprand1.to_ulong() + oprand2.to_ulong());
                  } 
                 if (ALUOP.to_ulong() == SUBU)
                 {
                 	ALUresult = bitset<32> (oprand1.to_ulong() - oprand2.to_ulong());
                 }
                 if (ALUOP.to_ulong() == AND)
                 {
                 	ALUresult = bitset<32> (oprand1.to_ulong() & oprand2.to_ulong());
                 }
                 if (ALUOP.to_ulong() == OR)
                 {
                 	ALUresult = bitset<32> (oprand1.to_ulong() | oprand2.to_ulong());
                 }
                 if (ALUOP.to_ulong() == NOR)
                 {
                 	ALUresult = bitset<32> (~(oprand1.to_ulong() | oprand2.to_ulong()));
                 }
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          	template <int N1, int N2 >		//Concatenation of each n1,n2 bit(s) instruction
			bitset <N1 + N2> concat( const bitset <N1> & b1, const bitset <N2> & b2 ) {
    				string s1 = b1.to_string();
    				string s2 = b2.to_string();
    				return bitset <N1 + N2>( s1 + s2 );
				}			


          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {   
				
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
              	
				long x = ReadAddress.to_ulong(); //ReadAddress starts at pc and reads next 3 bytes :D
               	bitset<16> temp16;
               	bitset<24> temp24;
               	temp16 = concat<8 , 8>(IMem[x], IMem[x+1]);
               	temp24 = concat<16 , 8>(temp16, IMem[x+2]);
               	Instruction = concat<24,8>(temp24, IMem[x+3]);

               	return Instruction;     
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  

          template <int N1, int N2 , int N3, int N4>		//Concatenation of each n1,n2,n3,n4 bit(s) instruction
			bitset <N1 + N2 + N3 + N4> concat( const bitset <N1> & b1, const bitset <N2> & b2 ,const bitset <N3> & b3 ,const bitset <N4> & b4 ) {
    				string s1 = b1.to_string();
    				string s2 = b2.to_string();
    				string s3 = b3.to_string();
    				string s4 = b4.to_string();
    				return bitset <N1 + N2 + N3 + N4>( s1 + s2 + s3 + s4 );
				}			


          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               //implement by you
         		string temp = WriteData.to_string();
               if (readmem.to_ulong() == 1)	
               {
               	readdata = concat<8,8,8,8>(DMem[Address.to_ulong()+0], DMem[Address.to_ulong()+1], DMem[Address.to_ulong()+2], DMem[Address.to_ulong()+3]);
               }
               if (writemem.to_ulong() == 1)
               {
               	DMem[Address.to_ulong()] = bitset<8> (temp.substr(0,8));
               	DMem[Address.to_ulong()+1] = bitset<8> (temp.substr(9,8));
               	DMem[Address.to_ulong()+2] = bitset<8> (temp.substr(17,8));
               	DMem[Address.to_ulong()+3] = bitset<8> (temp.substr(25,8));
               }
               return readdata;    
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  


//Coding is a bit amateur-styled, thanks for bearing :D
//Used "cout" just for better way to understand the I/O

int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    bitset<32> CurrIns;
    bitset<32> pc;
    
    bitset<6> opcode;
    bitset<5> rs;
    bitset<5> rt;
    bitset<5> rd; 
    bitset<5> shamt;
    bitset<6> fn;
    bitset<16> imm;
    bitset<26> jaddress;
    bitset<32> result;
	bitset<32> x;

    string InsCopy;
    string exe, temp;

    while (1)
	{
		
        //1.  Fetch
        CurrIns = myInsMem.ReadMemory(pc);
        

		//2. If current insturciton is "11111111111111111111111111111111", then break;
		if (CurrIns.to_string() == "11111111111111111111111111111111")
			break;
        

		//3. decode(Read RF)
			//Copying bitset instruction in string format for making it easy to compute
		InsCopy = CurrIns.to_string();

		cout << "\nInstruction : " << InsCopy << '\n';

    	opcode = bitset<6> (InsCopy.substr(0,6));

    	//Used if-else to identify instruction(s) and assign appropriate values
    	//Also helps ignoring bad input :D
    	if (opcode.to_string() == "111111")	//another halt[hex(3F)] instructions from pdf
    	{
    		/* code */
    	}


    	if (opcode.to_string() == "000000")	//R-type
    	{
    		rs = bitset<5> (InsCopy.substr(6,5));
    		rt = bitset<5> (InsCopy.substr(11,5));
    		rd = bitset<5> (InsCopy.substr(16,5)); 
    		shamt = bitset<5> (InsCopy.substr(21,5));
    		fn= bitset<6> (InsCopy.substr(26,6));

    		myRF.ReadWrite(rs, rt, rd, bitset<32> (0), bitset<1> (0)); //Reading from RF

    		cout << "opcode " << opcode << '\n';
    		cout << "rs " << rs<< '\n';
    		cout << " rt " << rt << '\n';
    		cout << " rd " << rd << '\n';
    		cout << " shamt" << shamt <<'\n';
    		cout << " fn " << fn << '\n';
    		exe = "r"; //set as r-type which helps in switching, same for rest of the instruction(s) type(s)

    	}
    	else if (opcode.to_string() == "000010" || opcode.to_string() == "000011")	//J-type
    	{
    		jaddress = bitset<26> (InsCopy.substr(6,26));

    		cout << "opcode " << opcode << '\n';
    		exe = "j";
    	}
    	else{	//I-type
    		rs = bitset<5> (InsCopy.substr(6,5));
    		rt = bitset<5> (InsCopy.substr(11,5));
    		imm = bitset<16> (InsCopy.substr(16,16));
    		
    		cout << "opcode " << opcode << '\n';
    		cout << " rs " << rs << '\n';
    		cout << " rt " << rt << '\n';
    		cout << " imm " << imm << '\n';
    		exe = "i";
    	}


		//4. Execute
    	if(exe == "r"){
    		//Execution of R-type instruction(s)

    	   		switch(fn.to_ulong()){

    			case 33:	//decimal for hex(21) which is "addu" operation
    						result = myALU.ALUOperation(bitset<3>(ADDU), myRF.ReadData1, myRF.ReadData2);
    						break;

    			case 35:	//decimal for hex(23) which is "subu" operation
    						result = myALU.ALUOperation(bitset<3>(SUBU), myRF.ReadData1, myRF.ReadData2);
    						break;

    			case 36:	//decimal for hex(24) which is "and" operation
    						result = myALU.ALUOperation(bitset<3>(AND), myRF.ReadData1, myRF.ReadData2);
    						break;

    			case 37:	//decimal for hex(25) which is "or" operation
    						result = myALU.ALUOperation(bitset<3>(OR), myRF.ReadData1, myRF.ReadData2);
    						break;
    			case 39:	//decimal for hex(27) which is "nor" operation
    						result = myALU.ALUOperation(bitset<3>(NOR), myRF.ReadData1, myRF.ReadData2);
    						break;

    			default:	//If something else is passed, doing nothing!
    						break;
    		}
    		myRF.ReadWrite(rs, rt, rd, result, bitset<1> (1)); //Writing back to RF
    	}

    	if (exe == "i"){
    		//Execution of I-type instruction(s)

    		switch(opcode.to_ulong()){
				case 35:	//decimal for hex(23)
							//which is "lw" instruction
							//doubt rs 1st or imm?
							//conflict with slides(ppT) & those green sheets

							if((imm.to_string()).substr(0,1) == "0"){
								temp = "0000000000000000";		
							}
							else {
								temp = "1111111111111111";
							}
							temp = temp + (imm.to_string());
							myRF.ReadWrite(rs, bitset<5> (0), bitset<5> (0), bitset<32> (0), bitset<1> (0));

							x = bitset<32> (temp);

							x = (x.to_ulong() + (myRF.ReadData1).to_ulong()); //imm = sign extended(imm) + valueAt(rs)

							result = myDataMem.MemoryAccess(x, bitset<32> (0), bitset<1> (1), bitset<1> (0));
							myRF.ReadWrite(bitset<5> (0), bitset<5> (0), rt, result, bitset<1> (1));
							cout << " lw " << '\n'; 
							break;

				case 43:	//decimal for hex(2b)
							//which is "sw" instruction
							
							if((imm.to_string()).substr(0,1) == "0"){
								temp = "0000000000000000";		
							}
							else {
								temp = "1111111111111111";
							}
							temp = temp + (imm.to_string());
							myRF.ReadWrite(rs, rt, bitset<5> (0), bitset<32> (0), bitset<1> (0));

							x = bitset<32> (temp);

							x = (x.to_ulong() + (myRF.ReadData1).to_ulong()); //imm = sign extended(imm) + valueAt(rs)

							result = myDataMem.MemoryAccess(x, myRF.ReadData2, bitset<1> (0), bitset<1> (1));
							cout << " sw " << '\n';
							break;

				case 9:		//decimal for hex(9)
							//which is "addiu" instruction
							//rt = rs + imm
							if((imm.to_string()).substr(0,1) == "0"){
								temp = "0000000000000000";		//sign extending, which is 16bit now
							}
							else {
								temp = "1111111111111111";
							}
							temp = temp + (imm.to_string());
							myRF.ReadWrite(rs, bitset<5> (0), rt, bitset<32> (0), bitset<1> (0));
							result = myALU.ALUOperation(bitset<3>(ADDU), myRF.ReadData1, bitset<32> (temp));
							myRF.ReadWrite(rs, rs, rt, result, bitset<1> (1));
							cout << " addiu " << '\n';
							break;

				case 4:		//decimal for hex(4)
							//which is "beq" instrcution
							// beq rs,rt,imm
							// if rs == rt then pc= pc + imm
							// else pc = pc + 4
							if((imm.to_string()).substr(0,1) == "0"){
								temp = "00000000000000";		//sign extending, which is 14bit now
							}
							else {
								temp = "11111111111111";
							}
							temp = temp + imm.to_string();	//30 bits now; 14bit(temp) + 16bits(imm)
							temp = temp + "00";				//32 bits now, kudos :D
							x = bitset<32> (temp);

							myRF.ReadWrite(rs, rt, bitset<5> (0), bitset<32> (0), bitset<1> (0));

							if (myRF.ReadData1 == myRF.ReadData2)	// if valueAt(rs) == value(rt) then pc = pc + 4 + imm
							{
								pc = (pc.to_ulong() + 4 + x.to_ulong());	//doing pc = pc + 4 + imm(sign extended as x)
							}
							break;

				default:	//If something else is passed, doing nothing!
    						break;
    			
    		}

		}

		if(exe == "j"){

			switch(opcode.to_ulong()){

				case 2:		//Decimal for hex(2) which is jump instruction
							//jump to concatenation of (PC[31:28], JumpAddres, 00)
							//So, concatenated as follows:
							//Eg: 11110000000000001111000000000000 in binary
							//Then the MSB[31:28] are "1111", in string format they're at [0 to 3] ergo {0,1,2,3} which is "1111"
							
							//According to slides, doing the PC+4 and then taking 4 MSB bits
							pc = (pc.to_ulong() + 4);
							temp = temp + (pc.to_string().substr(0 , 4));
							temp = temp + jaddress.to_string();
							temp = temp + "00";
							pc = bitset <32>(temp);

				default: 	//for illegal input, just in case :D
							break;


			}

		}

			//** Stages 5 and 6 are implemented in the switch-case for simplicity purpose **
		//5. Read/Write Mem
			

		//6. Write back to RF


		if (opcode.to_ulong() != 2 || opcode.to_ulong()!=4)
		{
			pc = (pc.to_ulong() + 4); //PC updation, when it's not an "Jump" or "Beq" Instruction
		}
			
		
        myRF.OutputRF(); // dump RF; 
         
    }
    	//cout << loopcount << '\n';
        myDataMem.OutputDataMem(); // dump data memw
      
        return 0;
        
}
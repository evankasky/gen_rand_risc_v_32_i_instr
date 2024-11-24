#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>
#include <cstdint>
#include <ctime>

//-------------------------------------------------
// Function Prototypes
//-------------------------------------------------
void initialize_register_map();
std::string select_random_register();
std::pair<std::string, uint32_t> gen_rand_instr();
std::pair<std::string, uint32_t> gen_rand_LUI();
std::pair<std::string, uint32_t> gen_rand_AUIPC();
std::pair<std::string, uint32_t> gen_rand_JAL();
std::pair<std::string, uint32_t> gen_rand_JALR();
std::pair<std::string, uint32_t> gen_rand_branch(const std::string& instr_name, uint32_t funct3);
std::pair<std::string, uint32_t> gen_rand_BEQ();
std::pair<std::string, uint32_t> gen_rand_BNE();
std::pair<std::string, uint32_t> gen_rand_BLT();
std::pair<std::string, uint32_t> gen_rand_BGE();
std::pair<std::string, uint32_t> gen_rand_BLTU();
std::pair<std::string, uint32_t> gen_rand_BGEU();
std::pair<std::string, uint32_t> gen_rand_load(const std::string& instr_name, uint32_t funct3);
std::pair<std::string, uint32_t> gen_rand_LB();
std::pair<std::string, uint32_t> gen_rand_LH();
std::pair<std::string, uint32_t> gen_rand_LW();
std::pair<std::string, uint32_t> gen_rand_LBU();
std::pair<std::string, uint32_t> gen_rand_LHU();
std::pair<std::string, uint32_t> gen_rand_store(const std::string& instr_name, uint32_t funct3);
std::pair<std::string, uint32_t> gen_rand_SB();
std::pair<std::string, uint32_t> gen_rand_SH();
std::pair<std::string, uint32_t> gen_rand_SW();
std::pair<std::string, uint32_t> gen_rand_immediate(const std::string& instr_name, uint32_t funct3);
std::pair<std::string, uint32_t> gen_rand_ADDI();
std::pair<std::string, uint32_t> gen_rand_SLTI();
std::pair<std::string, uint32_t> gen_rand_SLTIU();
std::pair<std::string, uint32_t> gen_rand_XORI();
std::pair<std::string, uint32_t> gen_rand_ORI();
std::pair<std::string, uint32_t> gen_rand_ANDI();
std::pair<std::string, uint32_t> gen_rand_shift(const std::string& instr_name, uint32_t funct3, uint32_t funct7);
std::pair<std::string, uint32_t> gen_rand_SLLI();
std::pair<std::string, uint32_t> gen_rand_SRLI();
std::pair<std::string, uint32_t> gen_rand_SRAI();
std::pair<std::string, uint32_t> gen_rand_register(const std::string& instr_name, uint32_t funct3, uint32_t funct7);
std::pair<std::string, uint32_t> gen_rand_ADD();
std::pair<std::string, uint32_t> gen_rand_SUB();
std::pair<std::string, uint32_t> gen_rand_SLL();
std::pair<std::string, uint32_t> gen_rand_SLT();
std::pair<std::string, uint32_t> gen_rand_SLTU();
std::pair<std::string, uint32_t> gen_rand_XOR();
std::pair<std::string, uint32_t> gen_rand_SRL();
std::pair<std::string, uint32_t> gen_rand_SRA();
std::pair<std::string, uint32_t> gen_rand_OR();
std::pair<std::string, uint32_t> gen_rand_AND();
uint32_t encode_R_type(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode);
uint32_t encode_I_type(uint32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode);
uint32_t encode_S_type(uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode);
uint32_t encode_B_type(uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode);
uint32_t encode_U_type(uint32_t imm, uint32_t rd, uint32_t opcode);
uint32_t encode_J_type(uint32_t imm, uint32_t rd, uint32_t opcode);


//-------------------------------------------------
// Define a map to associate instructions with their respective functions
//-------------------------------------------------

std::unordered_map<std::string, uint32_t> register_map;

std::unordered_map<std::string, std::function<std::pair<std::string, uint32_t>()>> instr_functions = {

        {"LUI", gen_rand_LUI},
        {"AUIPC", gen_rand_AUIPC},
        {"JAL", gen_rand_JAL},
        {"JALR", gen_rand_JALR},
        {"BEQ", gen_rand_BEQ},
        {"BNE", gen_rand_BNE},
        {"BLT", gen_rand_BLT},
        {"BGE", gen_rand_BGE},
        {"BLTU", gen_rand_BLTU},
        {"BGEU", gen_rand_BGEU},
        {"LB", gen_rand_LB},
        {"LH", gen_rand_LH},
        {"LW", gen_rand_LW},
        {"LBU", gen_rand_LBU},
        {"LHU", gen_rand_LHU},
        {"SB", gen_rand_SB},
        {"SH", gen_rand_SH},
        {"SW", gen_rand_SW},
        {"ADDI", gen_rand_ADDI},
        {"SLTI", gen_rand_SLTI},
        {"SLTIU", gen_rand_SLTIU},
        {"XORI", gen_rand_XORI},
        {"ORI", gen_rand_ORI},
        {"ANDI", gen_rand_ANDI},
        {"SLLI", gen_rand_SLLI},
        {"SRLI", gen_rand_SRLI},
        {"SRAI", gen_rand_SRAI},
        {"ADD", gen_rand_ADD},
        {"SUB", gen_rand_SUB},
        {"SLL", gen_rand_SLL},
        {"SLT", gen_rand_SLT},
        {"SLTU", gen_rand_SLTU},
        {"XOR", gen_rand_XOR},
        {"SRL", gen_rand_SRL},
        {"SRA", gen_rand_SRA},
        {"OR", gen_rand_OR},
        {"AND", gen_rand_AND}

    };


//-------------------------------------------------
// Main Function
//-------------------------------------------------

int main(int argc, char* argv[]) {

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize the register map
    initialize_register_map();

    for(int i = 0; i < 25; i++) {

        // Generate and output a random instruction
        std::pair<std::string, uint32_t> instr = gen_rand_instr();
        std::cout << instr.first << "\n" << std::hex << instr.second << "\n\n";

    }

    return 0;

}


//-------------------------------------------------
// Function Definitions
//-------------------------------------------------

void initialize_register_map() {

    for (uint32_t i = 0; i < 32; ++i) {

        register_map["x" + std::to_string(i)] = i;

    }

}

std::string select_random_register() {

    // Randomly select an index in the range [0, 31]
    int random_index = std::rand() % 32;

    // Iterate through the register_map to get the nth key
    int count = 0;

    std::string selected_register;

    for (std::pair<std::string, uint32_t> entry : register_map) {

        if (count == random_index) {

            selected_register = entry.first;
            break;

        }

        count++;

    }

    return selected_register;

}



std::pair<std::string, uint32_t> gen_rand_instr() {

    // Randomly select an instruction name from the instruction map keys
    auto it = instr_functions.begin();
    std::advance(it, std::rand() % instr_functions.size());
    std::string instr_name = it->first;

    // Call the corresponding instruction generation function
    return instr_functions[instr_name]();

}



std::pair<std::string, uint32_t> gen_rand_LUI() {

    // Randomly select an rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Generate a random 20-bit integer for the immediate field
    uint32_t imm = std::rand() & 0xFFFFF;

    // Set the opcode for the LUI instruction
    uint32_t opcode = 0b0110111;

    // Encode the instruction using the U-Type encoder
    uint32_t instruction = encode_U_type(imm, rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = "lui " + rd + ", " + std::to_string(imm);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};
    
}



std::pair<std::string, uint32_t> gen_rand_AUIPC(){

    // Randomly select an rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Generate a random 20-bit integer for the immediate field
    uint32_t imm = std::rand() & 0xFFFFF;

    // Set the opcode for the AUIPC instruction
    uint32_t opcode = 0b0010111;

    // Encode the instruction using the U-Type encoder
    uint32_t instruction = encode_U_type(imm, rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = "auipc " + rd + ", " + std::to_string(imm);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}



std::pair<std::string, uint32_t> gen_rand_JAL(){

    // Randomly select an rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Generate a random 21-bit signed immediate value
    int32_t offset = (std::rand() % (1 << 21)) - (1 << 20);

    // Set the opcode for the JAL instruction
    uint32_t opcode = 0b1101111;

    // Encode the instruction using the J-Type encoder
    uint32_t instruction = encode_J_type(static_cast<uint32_t>(offset), rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = "jal " + rd + ", " + std::to_string(offset);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}



std::pair<std::string, uint32_t> gen_rand_JALR() {

    // Randomly select an rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Randomly select an rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Generate a random signed 11-bit offset
    int32_t offset = (std::rand() % (1 << 12)) - (1 << 11);

    // Set the opcode for the JALR instruction
    uint32_t opcode = 0b1100111;

    // Set funct3 to 000 (per specification)
    uint32_t funct3 = 0b000;

    // Encode the instruction using the I-Type encoder
    uint32_t instruction = encode_I_type(static_cast<uint32_t>(offset), rs1_index, funct3, rd_index, opcode);

    // Create the assembly instruction string in the correct format
    std::string asm_str = "jalr " + rd + ", " + rs1 + ", " + std::to_string(offset);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}

std::pair<std::string, uint32_t> gen_rand_branch(const std::string& instr_name, uint32_t funct3) {

    // Randomly select rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Randomly select rs2 register
    std::string rs2 = select_random_register();
    uint32_t rs2_index = register_map[rs2];

    // Generate a signed random 12-bit number in the range [-4096, 4095]
    int32_t offset = (std::rand() % (1 << 12)) - (1 << 11);

    // Ensure the offset is even by clearing the least significant bit
    offset &= ~0x1;

    // Set the opcode for a branch type instruction
    uint32_t opcode = 0b1100011;

    // Encode the instruction using the B-Type encoder
    uint32_t instruction = encode_B_type(static_cast<uint32_t>(offset), rs2_index, rs1_index, funct3, opcode);

    // Generate the assembly instruction string
    std::string asm_str = instr_name + " " + rs1 + ", " + rs2 + ", " + std::to_string(offset);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}


std::pair<std::string, uint32_t> gen_rand_BEQ() {

    return gen_rand_branch("beq", 0b000);

}



std::pair<std::string, uint32_t> gen_rand_BNE() {

    return gen_rand_branch("bne", 0b001);

}


std::pair<std::string, uint32_t> gen_rand_BLT() {

    return gen_rand_branch("blt", 0b100);

}



std::pair<std::string, uint32_t> gen_rand_BGE() {

    return gen_rand_branch("bge", 0b101);

}



std::pair<std::string, uint32_t> gen_rand_BLTU() {

    return gen_rand_branch("bltu", 0b110);

}



std::pair<std::string, uint32_t> gen_rand_BGEU() {

    return gen_rand_branch("bgeu", 0b111);

}


std::pair<std::string, uint32_t> gen_rand_load(const std::string& instr_name, uint32_t funct3) {

    // Randomly select an rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Randomly select an rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Generate a random signed 11-bit offset in the range [-2048, 2047]
    int32_t offset = (std::rand() % (1 << 12)) - (1 << 11);

    // Set the opcode for a load instruction
    uint32_t opcode = 0b0000011;

    // Encode the instruction using the I-Type encoder
    uint32_t instruction = encode_I_type(static_cast<uint32_t>(offset), rs1_index, funct3, rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = instr_name + " " + rd + ", " + std::to_string(offset) + "(" + rs1 + ")";

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}


std::pair<std::string, uint32_t> gen_rand_LB() {

    return gen_rand_load("lb", 0b000);

}




std::pair<std::string, uint32_t> gen_rand_LH() {

    return gen_rand_load("lh", 0b001);

}



std::pair<std::string, uint32_t> gen_rand_LW() {

    return gen_rand_load("lw", 0b010);

}



std::pair<std::string, uint32_t> gen_rand_LBU() {
    
    return gen_rand_load("lbu", 0b100);

}


std::pair<std::string, uint32_t> gen_rand_LHU() {

    return gen_rand_load("lhu", 0b101);

}


std::pair<std::string, uint32_t> gen_rand_store(const std::string& instr_name, uint32_t funct3) {

    // Randomly select rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Randomly select rs2 register
    std::string rs2 = select_random_register();
    uint32_t rs2_index = register_map[rs2];

    // Generate a random signed 11-bit offset in the range [-2048, 2047]
    int32_t offset = (std::rand() % (1 << 12)) - (1 << 11);

    // Set the opcode for a store instruction
    uint32_t opcode = 0b0100011;

    // Encode the instruction using the S-Type encoder
    uint32_t instruction = encode_S_type(static_cast<uint32_t>(offset), rs2_index, rs1_index, funct3, opcode);

    // Create the assembly instruction string
    std::string asm_str = instr_name + " " + rs2 + ", " + std::to_string(offset) + "(" + rs1 + ")";

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}



std::pair<std::string, uint32_t> gen_rand_SB() {

    return gen_rand_store("sb", 0b000);

}



std::pair<std::string, uint32_t> gen_rand_SH() {

    return gen_rand_store("sh", 0b001);

}



std::pair<std::string, uint32_t> gen_rand_SW() {

    return gen_rand_store("sw", 0b010);

}


std::pair<std::string, uint32_t> gen_rand_immediate(const std::string& instr_name, uint32_t funct3) {

    // Randomly select rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Randomly select rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Generate a random signed 11-bit immediate in the range [-2048, 2047]
    int32_t imm = (std::rand() % (1 << 12)) - (1 << 11);

    // Set the opcode for an immediate instruction
    uint32_t opcode = 0b0010011;

    // Encode the instruction using the I-Type encoder
    uint32_t instruction = encode_I_type(static_cast<uint32_t>(imm), rs1_index, funct3, rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = instr_name + " " + rd + ", " + rs1 + ", " + std::to_string(imm);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}



std::pair<std::string, uint32_t> gen_rand_ADDI() {

    return gen_rand_immediate("addi", 0b000);

}



std::pair<std::string, uint32_t> gen_rand_SLTI() {

    return gen_rand_immediate("slti", 0b010);

}



std::pair<std::string, uint32_t> gen_rand_SLTIU() {

    return gen_rand_immediate("sltiu", 0b011);

}



std::pair<std::string, uint32_t> gen_rand_XORI() {

    return gen_rand_immediate("xori", 0b100);

}


std::pair<std::string, uint32_t> gen_rand_ORI() {

    return gen_rand_immediate("ori", 0b110);

}



std::pair<std::string, uint32_t> gen_rand_ANDI() {

    return gen_rand_immediate("andi", 0b111);

}

std::pair<std::string, uint32_t> gen_rand_AND() {

    return gen_rand_register("and", 0b111, 0b0000000);

}


std::pair<std::string, uint32_t> gen_rand_shift(const std::string& instr_name, uint32_t funct3, uint32_t funct7) {

    // Randomly select rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Randomly select rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Generate a random unsigned 5-bit shift amount in the range [0, 31]
    uint32_t shamt = std::rand() % 32;

    // Set the opcode for a shift instruction
    uint32_t opcode = 0b0010011;

    // Combine funct7 and shamt for the immediate field
    uint32_t imm = (funct7 << 5) | shamt;

    // Encode the instruction using the I-Type encoder
    uint32_t instruction = encode_I_type(imm, rs1_index, funct3, rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = instr_name + " " + rd + ", " + rs1 + ", " + std::to_string(shamt);

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}



std::pair<std::string, uint32_t> gen_rand_SLLI() {

    return gen_rand_shift("slli", 0b001, 0b0000000);

}




std::pair<std::string, uint32_t> gen_rand_SRLI() {

    return gen_rand_shift("srli", 0b101, 0b0000000);

}



std::pair<std::string, uint32_t> gen_rand_SRAI() {

    return gen_rand_shift("srai", 0b101, 0b0100000);

}

std::pair<std::string, uint32_t> gen_rand_register(const std::string& instr_name, uint32_t funct3, uint32_t funct7) {

    // Randomly select rd register
    std::string rd = select_random_register();
    uint32_t rd_index = register_map[rd];

    // Randomly select rs1 register
    std::string rs1 = select_random_register();
    uint32_t rs1_index = register_map[rs1];

    // Randomly select rs2 register
    std::string rs2 = select_random_register();
    uint32_t rs2_index = register_map[rs2];

    // Set the opcode for a register instruction
    uint32_t opcode = 0b0110011;

    // Encode the instruction using the R-Type encoder
    uint32_t instruction = encode_R_type(funct7, rs2_index, rs1_index, funct3, rd_index, opcode);

    // Create the assembly instruction string
    std::string asm_str = instr_name + " " + rd + ", " + rs1 + ", " + rs2;

    // Return the assembly string and the encoded instruction
    return {asm_str, instruction};

}



std::pair<std::string, uint32_t> gen_rand_ADD() {

    return gen_rand_register("add", 0b000, 0b0000000);

}


std::pair<std::string, uint32_t> gen_rand_SUB() {

    return gen_rand_register("sub", 0b000, 0b0100000);

}



std::pair<std::string, uint32_t> gen_rand_SLL() {

    return gen_rand_register("sll", 0b001, 0b0000000);

}



std::pair<std::string, uint32_t> gen_rand_SLT() {

    return gen_rand_register("slt", 0b010, 0b0000000);

}



std::pair<std::string, uint32_t> gen_rand_SLTU() {

    return gen_rand_register("sltu", 0b011, 0b0000000);

}



std::pair<std::string, uint32_t> gen_rand_XOR() {

    return gen_rand_register("xor", 0b100, 0b0000000);

}




std::pair<std::string, uint32_t> gen_rand_SRL() {

    return gen_rand_register("srl", 0b101, 0b0000000);

}




std::pair<std::string, uint32_t> gen_rand_SRA() {

    return gen_rand_register("sra", 0b101, 0b0100000);

}




std::pair<std::string, uint32_t> gen_rand_OR() {

    return gen_rand_register("or", 0b110, 0b0000000);

}



uint32_t encode_R_type(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode) {

    return ((funct7 & 0x7F) << 25) |        // funct7 field (7 bits)
           ((rs2 & 0x1F) << 20)  |          // rs2 field (5 bits)
           ((rs1 & 0x1F) << 15)  |          // rs1 field (5 bits)
           ((funct3 & 0x7) << 12) |         // funct3 field (3 bits)
           ((rd & 0x1F) << 7)    |          // rd field (5 bits)
           (opcode & 0x7F);                 // opcode field (7 bits)
    
}



uint32_t encode_I_type(uint32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode) {

    return ((imm & 0xFFF) << 20) |          // Immediate field (12 bits)
            ((rs1 & 0x1F) << 15)  |         // rs1 field (5 bits)
            ((funct3 & 0x7) << 12) |        // funct3 field (3 bits)
            ((rd & 0x1F) << 7)    |         // rd field (5 bits)
            (opcode & 0x7F);                // opcode field (7 bits)
    
}



uint32_t encode_S_type(uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode) {

    uint32_t imm11_5 = (imm >> 5) & 0x7F;   // Extract bits [11:5] of immediate
    uint32_t imm4_0 = imm & 0x1F;           // Extract bits [4:0] of immediate

    return (imm11_5 << 25) |                // imm[11:5] field (7 bits)
           ((rs2 & 0x1F) << 20) |           // rs2 field (5 bits)
           ((rs1 & 0x1F) << 15) |           // rs1 field (5 bits)
           ((funct3 & 0x7) << 12) |         // funct3 field (3 bits)
           (imm4_0 << 7) |                  // imm[4:0] field (5 bits)
           (opcode & 0x7F);                 // opcode field (7 bits)
    
}



uint32_t encode_B_type(uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode) {

    uint32_t imm12 = (imm >> 12) & 0x1;     // Extract bit 12
    uint32_t imm10_5 = (imm >> 5) & 0x3F;   // Extract bits [10:5]
    uint32_t imm4_1 = (imm >> 1) & 0xF;     // Extract bits [4:1]
    uint32_t imm11 = (imm >> 11) & 0x1;     // Extract bit 11

    return (imm12 << 31) |                  // imm[12] field (1 bit)
           (imm10_5 << 25) |                // imm[10:5] field (6 bits)
           ((rs2 & 0x1F) << 20) |           // rs2 field (5 bits)
           ((rs1 & 0x1F) << 15) |           // rs1 field (5 bits)
           ((funct3 & 0x7) << 12) |         // funct3 field (3 bits)
           (imm4_1 << 8) |                  // imm[4:1] field (4 bits)
           (imm11 << 7) |                   // imm[11] field (1 bit)
           (opcode & 0x7F);                 // opcode field (7 bits)
    
}



uint32_t encode_U_type(uint32_t imm, uint32_t rd, uint32_t opcode) {

    return ((imm & 0xFFFFF) << 12) |        // Immediate field (20 bits)
           ((rd & 0x1F) << 7) |             // rd field (5 bits)
           (opcode & 0x7F);                 // opcode field (7 bits)
    
}



uint32_t encode_J_type(uint32_t imm, uint32_t rd, uint32_t opcode) {

    uint32_t imm20 = (imm >> 20) & 0x1;         // Extract bit 20
    uint32_t imm10_1 = (imm >> 1) & 0x3FF;      // Extract bits [10:1]
    uint32_t imm11 = (imm >> 11) & 0x1;         // Extract bit 11
    uint32_t imm19_12 = (imm >> 12) & 0xFF;     // Extract bits [19:12]

    return (imm20 << 31) |                      // imm[20] field (1 bit)
           (imm19_12 << 12) |                   // imm[19:12] field (8 bits)
           (imm11 << 20) |                      // imm[11] field (1 bit)
           (imm10_1 << 21) |                    // imm[10:1] field (10 bits)
           ((rd & 0x1F) << 7) |                 // rd field (5 bits)
           (opcode & 0x7F);                     // opcode field (7 bits)
    
}


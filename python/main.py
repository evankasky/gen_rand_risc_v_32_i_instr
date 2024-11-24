import random
import time

# Ensures different random sequences every time the program runs
random.seed(time.time())

# Mapping of register names to numbers
register_map = {f'x{i}': i for i in range(32)}

#-------------------------------------------------------------------------------------
# Encoding Functions - Reference: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
#-------------------------------------------------------------------------------------

# Encode a R-Type instruction
# Format: funct7[31:25] | rs2[24:20] | rs1[19:15] | funct3[14:12] | rd[11:7] | opcode[6:0]
def encode_R_type(funct7, rs2, rs1, funct3, rd, opcode):

    instruction = (
        (funct7 & 0x7F) << 25 |                             # funct7 field (7 bits)
        (rs2 & 0x1F) << 20 |                                # rs2 field (5 bits)
        (rs1 & 0x1F) << 15 |                                # rs1 field (5 bits)
        (funct3 & 0x7) << 12 |                              # funct3 field (3 bits)  
        (rd & 0x1F) << 7 |                                  # rd field (5 bits)
        (opcode & 0x7F)                                     # opcode field (7 bits) 
    )

    return instruction

# Encode a I-Type instruction
# Format: imm[31:20] | rs1[19:15] | funct3[14:12] | rd[11:7] | opcode[6:0]
def encode_I_type(imm, rs1, funct3, rd, opcode):

    instruction = (
        (imm & 0xFFF) << 20 |                               # Immediate field (12 bits)
        (rs1 & 0x1F) << 15 |                                # rs1 field (5 bits)
        (funct3 & 0x7) << 12 |                              # funct3 field (3 bits)
        (rd & 0x1F) << 7 |                                  # rd field (5 bits)
        (opcode & 0x7F)                                     # opcode field (7 bits)
    )

    return instruction

# Encode a S-Type instruction
# Format: imm[11:5] | rs2[24:20] | rs1[19:15] | funct3[14:12] | imm[4:0] | opcode[6:0]
def encode_S_type(imm, rs2, rs1, funct3, opcode):

    imm11_5 = (imm >> 5) & 0x7F                             # Extract bits [11:5] of immediate
    imm4_0 = imm & 0x1F                                     # Extract bits [4:0] of immediate

    instruction = (
        (imm11_5) << 25 |                                   # imm[11:5] field (7 bits)
        (rs2 & 0x1F) << 20 |                                # rs2 field (5 bits)
        (rs1 & 0x1F) << 15 |                                # rs1 field (5 bits)
        (funct3 & 0x7) << 12 |                              # funct3 field (3 bits)
        (imm4_0) << 7 |                                     # imm[4:0] field (5 bits)
        (opcode & 0x7F)                                     # opcode field (7 bits)
    )

    return instruction

# Encode a B-Type instruction
# Format: imm[12|10:5|4:1|11] | rs2[24:20] | rs1[19:15] | funct3[14:12] | opcode[6:0]
def encode_B_type(imm, rs2, rs1, funct3, opcode):

    imm12 = (imm >> 12) & 0x1                               # Extract bit 12
    imm10_5 = (imm >> 5) & 0x3F                             # Extract bits [10:5]
    imm4_1 = (imm >> 1) & 0xF                               # Extract bits [4:1]
    imm11 = (imm >> 11) & 0x1                               # Extract bit 11

    instruction = (
        (imm12) << 31 |                                     # imm[12] field (1 bit)
        (imm10_5) << 25 |                                   # imm[10:5] field (6 bits)
        (rs2 & 0x1F) << 20 |                                # rs2 field (5 bits)
        (rs1 & 0x1F) << 15 |                                # rs1 field (5 bits)
        (funct3 & 0x7) << 12 |                              # funct3 field (3 bits)
        (imm4_1) << 8 |                                     # imm[4:1] field (4 bits)
        (imm11) << 7 |                                      # imm[11] field (1 bit)
        (opcode & 0x7F)                                     # opcode field (7 bits)
    )

    return instruction

# Encode a U-type instruction
# U-type format: imm[31:12] | rd[11:7] | opcode[6:0]
def encode_U_type(imm, rd, opcode):

    instruction = (
        (imm & 0xFFFFF) << 12 |                             # Immediate field (20 bits)
        (rd & 0x1F) << 7 |                                  # rd field (5 bits)
        (opcode & 0x7F)                                     # opcode field (7 bits)
    )

    return instruction

# Encode a J-type instruction
# J-type format: imm[20|10:1|11|19:12] | rd[11:7] | opcode[6:0]
def encode_J_type(imm, rd, opcode):

    imm20 = (imm >> 20) & 0x1                               # Extract bit 20
    imm10_1 = (imm >> 1) & 0x3FF                            # Extract bits [10:1]
    imm11 = (imm >> 11) & 0x1                               # Extract bit 11
    imm19_12 = (imm >> 12) & 0xFF                           # Extract bits [19:12]

    instruction = (
        (imm20) << 31 |                                     # imm[20] field (1 bit)
        (imm19_12) << 12 |                                  # imm[19:12] field (8 bits)
        (imm11) << 20 |                                     # imm[11] field (1 bit)
        (imm10_1) << 21 |                                   # imm[10:1] field (10 bits)
        (rd & 0x1F) << 7 |                                  # rd field (5 bits)
        (opcode & 0x7F)                                     # opcode field (7 bits)
    )
    
    return instruction

# Instruction generator functions with encoding

#-------------------------------------------------------------------------------------
# Instruction Generators
#-------------------------------------------------------------------------------------

# Generate a random LUI instruction
def gen_rand_LUI():

    # Get a random rd register for the instruction from the register map
    rd = random.choice(list(register_map.keys()))

    # Generate a random 20-bit integer for the imm field
    imm = random.randint(0, 0xFFFFF)

    # Set the opcode for the LUI instr
    opcode = 0b0110111

    # Use the U-Type instruction encoder with the random values generated above to get the instruction
    instruction = encode_U_type(imm, register_map[rd], opcode)

    # Create the asm instruction string
    asm = f"lui {rd}, {imm}"

    # Return the asm string and the encoded instruction
    return asm, instruction

# Generate a random AUIPC instr
def gen_rand_AUIPC():

    # Get a random rd register for the instruction from the register map
    rd = random.choice(list(register_map.keys()))

    # Generate a random 20-bit integer for the imm field
    imm = random.randint(0, 0xFFFFF)

    # Set the opcode for the AUIPC instr
    opcode = 0b0010111

    # Use the U-Type encoder function with the random generated values above to create the instr
    instruction = encode_U_type(imm, register_map[rd], opcode)

    # Create the asm string
    asm = f"auipc {rd}, {imm}"

    # Teutnr the asm string and the encoded instr
    return asm, instruction

# Generate a random JAL instr
def gen_rand_JAL():

    # Get a random rd register for the instruction from the register map
    rd = random.choice(list(register_map.keys()))

    # Generate a random 21-bit signed imm value
    # -2^20    = smallest signed 21-bit number
    # 2^20 - 1 = largest signed 21-bit number
    offset = random.randint(-2**20, 2**20 - 1)

    # Set the opcode for the JAL instr
    opcode = 0b1101111

    # Encode the J-Type instruction with the random values generated above
    instruction = encode_J_type(offset, register_map[rd], opcode)

    # Create the ams version string of the instruction
    asm = f"jal {rd}, {offset}"

    # Return the asm string, and the encoded instr
    return asm, instruction

# Generate a random JALR instr
def gen_rand_JALR():

    # Get a random register for the rd feild of the instruction from the register map
    rd = random.choice(list(register_map.keys()))

    # Get a random register for the rs1 field of the instruction from the register map
    rs1 = random.choice(list(register_map.keys()))

    # Generate a random signed 11-bit number for the offset
    # -2048 = smallest signed 11-bit num
    # 2047  = largest signed 11-bit num
    offset = random.randint(-2048, 2047)

    # Set the opcode for the JALR instr
    opcode = 0b1100111

    # Set the funct3 to 000 according to reference: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    funct3 = 0b000

    # Encode the instruction with the I-type encoder to get the instrucution
    instruction = encode_I_type(offset, register_map[rs1], funct3, register_map[rd], opcode)
    
    # Create the ams string version of the instruction
    asm = f"jalr {rd}, {offset}({rs1})"

    # Return the asm stirng and the encoded instruction
    return asm, instruction

# Generate a random branch
def gen_rand_branch(instr_name, funct3):
    
    # Get a random register for the rs1 field in the instruction
    rs1 = random.choice(list(register_map.keys()))

    # Get a random register for the rs2 field in the instruction
    rs2 = random.choice(list(register_map.keys()))

    # Generate a signed random 12-bit number that is even
    # -4096 = smallest even signed 12-bit number
    # 4094  = largest even signed 12-bit number
    offset = random.randint(-4096, 4094)

    # Make the LSB zero to ensure even
    offset &= ~0x1

    # Set the opcode for a branch type instr
    opcode = 0b1100011

    # Encode the instrution using the B-Type encoder to get the instruction
    instruction = encode_B_type(offset, register_map[rs2], register_map[rs1], funct3, opcode)

    # Generate the asm string version of the instruction
    asm = f"{instr_name} {rs1}, {rs2}, {offset}"

    # Return the asm strnig and the encoded instr
    return asm, instruction

# Generate a random BEQ instr
def gen_rand_BEQ():

    # Use the gen_rand_branch function with the correct funct3 from ref: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    return gen_rand_branch("beq", 0b000)

def gen_rand_BNE():

    # Use the gen_rand_branch function with the correct funct3 from ref: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    return gen_rand_branch("bne", 0b001)

def gen_rand_BLT():

    # Use the gen_rand_branch function with the correct funct3 from ref: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    return gen_rand_branch("blt", 0b100)

def gen_rand_BGE():

    # Use the gen_rand_branch function with the correct funct3 from ref: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    return gen_rand_branch("bge", 0b101)

def gen_rand_BLTU():

    # Use the gen_rand_branch function with the correct funct3 from ref: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    return gen_rand_branch("bltu", 0b110)

def gen_rand_BGEU():

    # Use the gen_rand_branch function with the correct funct3 from ref: https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf page: 104-105
    return gen_rand_branch("bgeu", 0b111)

# Generate a random load instruction
def gen_rand_load(instr_name, funct3):

    # Get a random register to use as the rd value
    rd = random.choice(list(register_map.keys()))
    
    # Get a random register to use as the rs1 value
    rs1 = random.choice(list(register_map.keys()))

    # Generate a random signed 11-bit offeset value
    # -2048 = smallest signed 11-bit value
    # 2047  = largest signed 11-bit value
    offset = random.randint(-2048, 2047)

    # Set the opcode for a load instr
    opcode = 0b0000011

    # Encode the instruction using the I-Type encoder with the random values generated above
    instruction = encode_I_type(offset, register_map[rs1], funct3, register_map[rd], opcode)

    # Create an string of the ams code
    asm = f"{instr_name} {rd}, {offset}({rs1})"

    # Return the asm string and the encoded instr
    return asm, instruction

# Genetate a random LB instr
def gen_rand_LB():

    # Use the gen_rand_load function with the proper funct3 value for lb
    return gen_rand_load("lb", 0b000)

def gen_rand_LH():

    # Use the gen_rand_load function with the proper funct3 value for lh
    return gen_rand_load("lh", 0b001)

def gen_rand_LW():

    # Use the gen_rand_load function with the proper funct3 value for lw
    return gen_rand_load("lw", 0b010)

def gen_rand_LBU():

    # Use the gen_rand_load function with the proper funct3 value for lbu
    return gen_rand_load("lbu", 0b100)

def gen_rand_LHU():

    # Use the gen_rand_load function with the proper funct3 value for lhu
    return gen_rand_load("lhu", 0b101)

# Generate a random store instruction
def gen_rand_store(instr_name, funct3):

    # Get a random register to use for the rs1 value in the register
    rs1 = random.choice(list(register_map.keys()))

    # Get a random register to use for the rs2 value in the register
    rs2 = random.choice(list(register_map.keys()))

    # Generate a random signed 11-bit offeset value
    # -2048 = smallest signed 11-bit value
    # 2047  = largest signed 11-bit value
    offset = random.randint(-2048, 2047)

    # Set the opcode for a store instr
    opcode = 0b0100011

    # Encode the store instr as a s-type instruction using the encoder with the random values
    instruction = encode_S_type(offset, register_map[rs2], register_map[rs1], funct3, opcode)

    # Generate an asm string of the instr
    asm = f"{instr_name} {rs2}, {offset}({rs1})"

    # Return the asm string and the encoded instr
    return asm, instruction

# Generate a random SB instr
def gen_rand_SB():

    # use the gen_rand_store instr function with the proper funct3 value for sb
    return gen_rand_store("sb", 0b000)

# Generate a random SH instr
def gen_rand_SH():

    # use the gen_rand_store instr function with the proper funct3 value for sh
    return gen_rand_store("sh", 0b001)

# Generate a raondom SW instr
def gen_rand_SW():

    # use the gen_rand_store instr function with the proper funct3 value for sw
    return gen_rand_store("sw", 0b010)

# Generate a ranodm immediate instr
def gen_rand_immediate(instr_name, funct3):

    # Get a random register to use as the rd value in the instr
    rd = random.choice(list(register_map.keys()))

    # Get a random register to use as the rs1 value in the instr
    rs1 = random.choice(list(register_map.keys()))

    # Generate a random signed 11-bit imm value
    # -2048 = smallest signed 11-bit value
    # 2047  = largest signed 11-bit value
    imm = random.randint(-2048, 2047)

    # Set the opcode for a imm instr
    opcode = 0b0010011

    # Encode the instruction using the I-Type encoder to get the instr
    instruction = encode_I_type(imm, register_map[rs1], funct3, register_map[rd], opcode)

    # Create the asm instr string
    asm = f"{instr_name} {rd}, {rs1}, {imm}"

    # Return the asm string and the encoded instr
    return asm, instruction

# Generate a rand ADDI instr
def gen_rand_ADDI():

    # Use the gen_rand_immediate func with the proper func3 value for ADDI
    return gen_rand_immediate("addi", 0b000)

# Generate a random SLTI instr
def gen_rand_SLTI():

    # Use the gen_rand_immediate func with the proper func3 value for SLTI
    return gen_rand_immediate("slti", 0b010)

# Generate a random SLTIU instr
def gen_rand_SLTIU():

    # Use the gen_rand_immediate func with the proper func3 value for SLTIU
    return gen_rand_immediate("sltiu", 0b011)

# Generate a random XORI instr
def gen_rand_XORI():

    # Use the gen_rand_immediate func with the proper func3 value for XORI
    return gen_rand_immediate("xori", 0b100)

# Generate a random ORI instr
def gen_rand_ORI():

    # Use the gen_rand_immediate func with the proper func3 value for ORI
    return gen_rand_immediate("ori", 0b110)

# Generate a random ANDI instr
def gen_rand_ANDI():

    # Use the gen_rand_immediate func with the proper func3 value for ANDI
    return gen_rand_immediate("andi", 0b111)

# Generate a random shift instr
def gen_rand_shift(instr_name, funct3, funct7):

    # Get a raondom register to use as the rd value for the instr
    rd = random.choice(list(register_map.keys()))

    # Get a random register to ise as the rs1 value for the instr
    rs1 = random.choice(list(register_map.keys()))

    # Generate a random unsigned 5-bit number for the shift amount
    # 0  = Smallest unsigned 5-bit number
    # 31 = Largest usigned 5-bit number
    shamt = random.randint(0, 31)

    # Set the opcode for a shift instr
    opcode = 0b0010011

    # Encode the instruction using the I-Type encoder with the random values
    instruction = encode_I_type((funct7 << 5) | shamt, register_map[rs1], funct3, register_map[rd], opcode)

    # Generate a string version of the instr
    asm = f"{instr_name} {rd}, {rs1}, {shamt}"

    # Return the asm string and the encoded instr
    return asm, instruction

# Generate a random SLLI instr
def gen_rand_SLLI():

    # Use the gen_rand_shift with the proper funct3 and funct7 for SLLI
    return gen_rand_shift("slli", 0b001, 0b0000000)

# Generate a random SRLI instr
def gen_rand_SRLI():

    # Use the gen_rand_shift with the proper funct3 and funct7 for SRLI
    return gen_rand_shift("srli", 0b101, 0b0000000)

# Generate a random SRAI instr
def gen_rand_SRAI():

    # Use the gen_rand_shift with the proper funct3 and funct7 for SRAI
    return gen_rand_shift("srai", 0b101, 0b0100000)

# Generate a random refsiter instruction
def gen_rand_register(instr_name, funct3, funct7):

    # Get a random register to use as the rd value in the instr
    rd = random.choice(list(register_map.keys()))

    # Get a random reigster to use as the rs1 value in the instr
    rs1 = random.choice(list(register_map.keys()))

    # Get a random reigster to use as the rs2 value in the instr
    rs2 = random.choice(list(register_map.keys()))

    # Set the opcode for a register instruction
    opcode = 0b0110011

    # Encode the instruction using the R-Type encoder with the proper funct3, and funct7, and the rand values from above
    instruction = encode_R_type(funct7, register_map[rs2], register_map[rs1], funct3, register_map[rd], opcode)

    # Create the asm string of the instr
    asm = f"{instr_name} {rd}, {rs1}, {rs2}"

    # Return the asm instr string and the encoded instr
    return asm, instruction

# Generate a random ADD instr
def gen_rand_ADD():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a ADD instr
    return gen_rand_register("add", 0b000, 0b0000000)

# Generate a random SUB instr
def gen_rand_SUB():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a SUB instr
    return gen_rand_register("sub", 0b000, 0b0100000)

# Generate a random SLL instr
def gen_rand_SLL():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a SLL instr
    return gen_rand_register("sll", 0b001, 0b0000000)

# Generate a random SLT instr
def gen_rand_SLT():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a SLT instr
    return gen_rand_register("slt", 0b010, 0b0000000)

# Generate a random SLTU instr
def gen_rand_SLTU():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a SLTU
    return gen_rand_register("sltu", 0b011, 0b0000000)

# Generate a random XOR instr
def gen_rand_XOR():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a XOR instr
    return gen_rand_register("xor", 0b100, 0b0000000)

# Generate a random SRL instr
def gen_rand_SRL():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a SRL instr
    return gen_rand_register("srl", 0b101, 0b0000000)

# Generate a random SRL instr
def gen_rand_SRA():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a SRL instr
    return gen_rand_register("sra", 0b101, 0b0100000)

# Generate a random OR instr
def gen_rand_OR():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a OR instr
    return gen_rand_register("or", 0b110, 0b0000000)

# Generate a random AND instr
def gen_rand_AND():

    # Use the gen_rand_register function with the proper funct3 and funct7 values for a AND instr
    return gen_rand_register("and", 0b111, 0b0000000)

#-------------------------------------------------------------------
# Create a map of instrutions with their respective functions
#-------------------------------------------------------------------

instr_functions = {
    'LUI': gen_rand_LUI,
    'AUIPC': gen_rand_AUIPC,
    'JAL': gen_rand_JAL,
    'JALR': gen_rand_JALR,
    'BEQ': gen_rand_BEQ,
    'BNE': gen_rand_BNE,
    'BLT': gen_rand_BLT,
    'BGE': gen_rand_BGE,
    'BLTU': gen_rand_BLTU,
    'BGEU': gen_rand_BGEU,
    'LB': gen_rand_LB,
    'LH': gen_rand_LH,
    'LW': gen_rand_LW,
    'LBU': gen_rand_LBU,
    'LHU': gen_rand_LHU,
    'SB': gen_rand_SB,
    'SH': gen_rand_SH,
    'SW': gen_rand_SW,
    'ADDI': gen_rand_ADDI,
    'SLTI': gen_rand_SLTI,
    'SLTIU': gen_rand_SLTIU,
    'XORI': gen_rand_XORI,
    'ORI': gen_rand_ORI,
    'ANDI': gen_rand_ANDI,
    'SLLI': gen_rand_SLLI,
    'SRLI': gen_rand_SRLI,
    'SRAI': gen_rand_SRAI,
    'ADD': gen_rand_ADD,
    'SUB': gen_rand_SUB,
    'SLL': gen_rand_SLL,
    'SLT': gen_rand_SLT,
    'SLTU': gen_rand_SLTU,
    'XOR': gen_rand_XOR,
    'SRL': gen_rand_SRL,
    'SRA': gen_rand_SRA,
    'OR': gen_rand_OR,
    'AND': gen_rand_AND
}

#-------------------------------------------------------
# Function to generate a random instruction
#-------------------------------------------------------

def generate_random_instruction():
    instr_name = random.choice(list(instr_functions.keys()))
    asm, instruction = instr_functions[instr_name]()
    return asm, instruction

#-------------------------------------------------------
# Main Function
#-------------------------------------------------------
if __name__ == "__main__":
    for _ in range(25):
        asm_code, machine_code = generate_random_instruction()
        print(asm_code)

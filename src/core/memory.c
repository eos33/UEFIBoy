#include "memory.h"
#include "memory_layout.h"
#include "romInfo.h"
#include "graphics.h"
#include "IO.h"
#include <stdio.h>
#include <string.h>

static uint8_t mem[WORD_MAX - 0x100];
  
static uint8_t oam
  
static uint8_t io_mem[256]= {
		0xCF, 0x00, 0x7E, 0xFF, 0xD3, 0x00, 0x00, 0xF8,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1,
		0x80, 0xBF, 0xF3, 0xFF, 0xBF, 0xFF, 0x3F, 0x00,
		0xFF, 0xBF, 0x7F, 0xFF, 0x9F, 0xFF, 0xBF, 0xFF,
		0xFF, 0x00, 0x00, 0xBF, 0x77, 0xF3, 0xF1, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x71, 0x72, 0xD5, 0x91, 0x58, 0xBB, 0x2A, 0xFA,
		0xCF, 0x3C, 0x54, 0x75, 0x48, 0xCF, 0x8F, 0xD9,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC,
		0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x2B, 0x0B, 0x64, 0x2F, 0xAF, 0x15, 0x60, 0x6D,
		0x61, 0x4E, 0xAC, 0x45, 0x0F, 0xDA, 0x92, 0xF3,
		0x83, 0x38, 0xE4, 0x4E, 0xA7, 0x6C, 0x38, 0x58,
		0xBE, 0xEA, 0xE5, 0x81, 0xB4, 0xCB, 0xBF, 0x7B,
		0x59, 0xAD, 0x50, 0x13, 0x5E, 0xF6, 0xB3, 0xC1,
		0xDC, 0xDF, 0x9E, 0x68, 0xD7, 0x59, 0x26, 0xF3,
		0x62, 0x54, 0xF8, 0x36, 0xB7, 0x78, 0x6A, 0x22,
		0xA7, 0xDD, 0x88, 0x15, 0xCA, 0x96, 0x39, 0xD3,
		0xE6, 0x55, 0x6E, 0xEA, 0x90, 0x76, 0xB8, 0xFF,
		0x50, 0xCD, 0xB5, 0x1B, 0x1F, 0xA5, 0x4D, 0x2E,
		0xB4, 0x09, 0x47, 0x8A, 0xC4, 0x5A, 0x8C, 0x4E,
		0xE7, 0x29, 0x50, 0x88, 0xA8, 0x66, 0x85, 0x4B,
		0xAA, 0x38, 0xE7, 0x6B, 0x45, 0x3E, 0x30, 0x37,
		0xBA, 0xC5, 0x31, 0xF2, 0x71, 0xB4, 0xCF, 0x29,
		0xBC, 0x7F, 0x7E, 0xD0, 0xC7, 0xC3, 0xBD, 0xCF,
		0x59, 0xEA, 0x39, 0x01, 0x2E, 0x00, 0x69, 0x00
};


static void io_set_mem(uint8_t addr, uint8_t val) {

    io_mem[addr] = val;

    /*OAM, store window x-coridinate priorities
     * Ready for rendering */
    if (addr > 0xFE00 && addr < 0xFEA0) {
        if(addr -1 % 4 == 0) { //Byte 1 is X Position
            update_sprite_prios((addr - 0xFE00) /4 ,val);
        } 
    }
    switch (global_addr) {
        
        /*  Timers */
        case P1_REG  : joypad_write(val); break;
        //case SC_REG : if (val == 0x81) {printf("%c",io_mem[GLOBAL_TO_IO_ADDR(SB_REG)]);} break;
        case TIMA_REG : break;
        case TMA_REG  : break;
        case TAC_REG  : break;
        /*  Attempting to set DIV reg resets it to 0 */
        case DIV_REG  : io_mem[io_addr] = 0; break;
        /*  Attempting to set LY reg resets it to 0  */
        case LY_REG   : break; // io_mem[io_addr] = 0; break;
        /*  Perform direct memory transfer  */
        case DMA_REG  : dma_transfer(val); break;
    }
}

           
    

/*  Gameboy bootstrap ROM for startup
 *  Modified from the original so that the CPU doesn't
 *  hang if the ROM checksum checksum is incorrect */
static uint8_t const dmg_boot_rom[0x100] = {
  0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb,
  0x21, 0x26, 0xff, 0x0e, 0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
  0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, 0x47, 0x11, 0x04, 0x01,
  0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
  0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22,
  0x23, 0x05, 0x20, 0xf9, 0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
  0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20, 0xf9, 0x2e, 0x0f, 0x18,
  0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
  0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20,
  0xf7, 0x1d, 0x20, 0xf2, 0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
  0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06, 0x7b, 0xe2, 0x0c, 0x3e,
  0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
  0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17,
  0xc1, 0xcb, 0x11, 0x17, 0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,
  0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83,
  0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
  0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63,
  0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,
  0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x21, 0x04, 0x01, 0x11,
  0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
  0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x00, 0x00,
  0x3e, 0x01, 0xe0, 0x50
};

// State of DMG OAM Ram on startup 
static uint8_t const oam_dump[0xA0] = {
    0xBB, 0xD8, 0xC4, 0x04, 0xCD, 0xAC, 0xA1, 0xC7,
    0x7D, 0x85, 0x15, 0xF0, 0xAD, 0x19, 0x11, 0x6A,
    0xBA, 0xC7, 0x76, 0xF8, 0x5C, 0xA0, 0x67, 0x0A,
    0x7B, 0x75, 0x56, 0x3B, 0x65, 0x5C, 0x4D, 0xA3,
    0x00, 0x05, 0xD7, 0xC9, 0x1B, 0xCA, 0x11, 0x6D,
    0x38, 0xE7, 0x13, 0x2A, 0xB1, 0x10, 0x72, 0x4D,
    0xA7, 0x47, 0x13, 0x89, 0x7C, 0x62, 0x5F, 0x90,
    0x64, 0x2E, 0xD3, 0xEF, 0xAB, 0x01, 0x15, 0x85,
    0xE8, 0x2A, 0x6E, 0x4A, 0x1F, 0xBE, 0x49, 0xB1,
    0xE6, 0x0F, 0x93, 0xE2, 0xB6, 0x87, 0x5D, 0x35,
    0xD8, 0xD4, 0x4A, 0x45, 0xCA, 0xB3, 0x33, 0x74,
    0x18, 0xC1, 0x16, 0xFB, 0x8F, 0xA4, 0x8E, 0x70,
    0xCD, 0xB4, 0x4A, 0xDC, 0xE6, 0x34, 0x32, 0x41,
    0xF9, 0x84, 0x6A, 0x99, 0xEC, 0x92, 0xF1, 0x8B,
    0x5D, 0xA5, 0x09, 0xCF, 0x3A, 0x93, 0xBC, 0xE0,
    0x15, 0x19, 0xE4, 0xB6, 0x9A, 0x04, 0x3B, 0xC1,
    0x96, 0xB7, 0x56, 0x85, 0x6A, 0xAA, 0x1E, 0x2A,
    0x80, 0xEE, 0xE7, 0x46, 0x76, 0x8B, 0x0D, 0xBA,
    0x24, 0x40, 0x42, 0x05, 0x0E, 0x04, 0x20, 0xA6,
    0x5E, 0xC1, 0x97, 0x7E, 0x44, 0x05, 0x01, 0xA9
};

/* Store first 255 bytes of cartridge so it can
   be restored after boot room has finished */
static uint8_t cartridge_start[0x100];

static unsigned rom_bank_count = 0; //
static int mbc_mode = 0; //memory bank mode

static uint8_t RAM_banks[4][0x2000];
static uint8_t ROM_banks[125][0x4000];// 125 16kb banks

static unsigned current_RAM_bank = 0;
static unsigned current_ROM_bank = 1;

static uint8_t bank_mode = 0;

// Get the memory bank mode of ROM loaded into memory
static void setup_MBC_mode() {
    uint8_t type = ROM_banks[0][CARTRIDGE_TYPE];

    if  (type == 0) {
        mbc_mode = MBC0;
    } else if (type >=1 || type <= 3) {
        mbc_mode = MBC1;
    } else if (type == 5 || type == 6) {
        mbc_mode = MBC2;
    } else {
        fprintf(stderr, "unknown rom memory type : %u\n",type);
    }
}

static void setup_RAM_banks() {
    // Only MBC1 has RAM banks
    if (mbc_mode == MBC1) {
        
    }

}
/* After initial 32kb of rom read into memory
 * mmu setup should occur */
void setup_mmu() {

    setup_MBC_mode();  
    setup_RAM_banks();      
    //Only MBC1 has RAM banks
    if (mbc_mode == MBC1) {

    }
}


int load_rom(char const *file_data, size_t size) {
    
    if (size < CARTRIDGE_ROM_SIZE + 1) {
        fprintf(stderr, "Error: Cartridge size is too small (%lu bytes)\n",size);
        return 0;
    }

    size_t rom_size = get_rom_size(file_data[CARTRIDGE_ROM_SIZE]) * 1024;

    // Data read in doesn't match header information
    if (size != rom_size) {
        fprintf(stderr, "Error: Cartridge header info on its size (%lu bytes) \
            doesn't match file size (%lu bytes)\n",rom_size, size);
        return 0;
    }

    rom_bank_count = rom_size / 0x4000;
    if (rom_bank_count <= 31) {
       bank_mode = 1; 
    }
   
    // Fill up ROM banks     
    for (unsigned n = 0; n < rom_bank_count; n++) {
        memcpy(ROM_banks[n], file_data + (0x4000 * n), 0x4000);
    }

    /*for (unsigned j = 0; j< 4; j++) {
    for (unsigned i = 0; i < 0x4000; i++) {
        printf("%x ",ROM_banks[j][i]);
    }*/

    setup_MBC_mode();
    /* Copy first 100 bytes of cartridge before
     * it is overwritten by the boot rom so we can restore
     *  it later */
    memcpy(cartridge_start, ROM_banks[0],  0x100);
    memcpy(ROM_banks[0], dmg_boot_rom, 0x100);
    
    // Load initial Sprite/Object attribute table values
    memcpy(mem + SPRITE_ATTRIBUTE_TABLE_START, oam_dump, 0xA0);

    return 1;
} 

/* Restore first 255 bytes of memory
   with first 255 bytes of the cartridge */
void unload_boot_rom() {
    memcpy(ROM_banks[0], cartridge_start, 0x100);
}


static int ram_banking = 0;
void set_MBC1_mem(uint16_t const addr, uint8_t const val) {


        // Setting ROM/RAM banking mode
        if(addr <= 0x1FFF) {
            ram_banking = (val & 0xF) == 0xA;
        // Setting ROM bank
        } else if((addr >= 0x2000) && (addr <= 0x3FFF)) {
             //printf("switching rom bank %d\n", current_ROM_bank); 
             current_ROM_bank = (val & 0x1F) + ((val & 0x1F) == 0);
           
        } else if((addr >= 0x4000) && (addr <= 0x5FFF)) { 
            current_RAM_bank = (val & 0x3); 
        } else if((addr >= 0x6000) && (addr <= 0x7FFF)) { 
            printf("switching bank mode: val %d new mode %d\n",val, val & 0x1);
            bank_mode = (val & 0x1); 
        } 
        //Write to External RAM (0xA000 - 0xBFFF)
         else if(((addr & 0xE000) == 0xA000)  && ram_banking) { 
            if(bank_mode == 0) { 
                RAM_banks[0][addr - 0xA000] = val; 
            } else {
                RAM_banks[current_RAM_bank][addr - 0xA000] = val; 
            }
        }
}





uint8_t get_MBC1_mem(uint16_t const addr) {

        if (addr < 0x4000) {
            return ROM_banks[0][addr];
        }
        //Read using ROM Banking 0x4000 - 0x87FFF
        if((addr & 0xC000) == 0x4000) {
            if (bank_mode == 0) { //Upper 2 bits of ROM bank speciffied by ram bits 
                //printf("Reading from bank %d\n", (current_RAM_bank << 5) | current_ROM_bank);
                return ROM_banks[(current_RAM_bank << 5) | 
                    current_ROM_bank][addr - 0x4000];
            } else {
                return ROM_banks[current_ROM_bank][addr - 0x4000];
            }
        }

        //Read using RAM Banking 0xA000 - 0xBFFF
        else if(((addr & 0xE000) == 0xA000) && ram_banking) {
            if (bank_mode == 0) {
                return RAM_banks[0][addr - 0xA000]; 
             } else { 
                return RAM_banks[current_RAM_bank][addr - 0xA000]; 
             } 
        }

        return 0x0;
}
 

/* Directly inject a value in memory without performing
 * checks, use carefully. Used for fast access or for
 * controllers which have direct access to that location
 * in memory in which the CPU does not*/
void set_mem_override(uint16_t loc, uint8_t val) {
   // mem[loc] = val;
   io_set_mem(GLOBAL_TO_IO_ADDR(loc), val);
}

void set_mem(uint16_t const addr, uint8_t const val) {
    
    if (mbc_mode == MBC1 && (addr < 0x8000 || (addr >= 0xA000 && addr < 0xC000))) {
        set_MBC1_mem(addr, val);
        return;
    }
    if (addr == 0xFF50 && val == 1) {
        printf("restoring rom header\n");
        unload_boot_rom();
    } 
    
    

    // Can't write to ROM banks or any memory below
    if (addr < 0x8000) {
        return;
    }


    if (addr < 0xFF00) {
        mem[addr] = val;
    }

    /*  Check if mirrored memory being written to */
    if (addr >= ECHO_RAM_START && addr <= ECHO_RAM_END) {
        mem[addr - 0x2000] = val;
    } else if (addr >= ECHO_RAM_START-0x2000 && addr <= ECHO_RAM_END-0x2000) {
        mem[addr + 0x2000] = val;
        //printf("echo2 loc %x new loc %x val %x\n",loc, loc+0x2000, val);
    }

    /*  IO being written to */
    if (addr >= 0xFF00) {
        io_set_mem(GLOBAL_TO_IO_ADDR(addr), val);
    }
}



uint8_t get_mem(uint16_t const addr) {
    if (mbc_mode == MBC1 && (addr < 0x8000 || (addr >= 0xA000 && addr < 0xC000))) {
         return get_MBC1_mem(addr);
    } else if (addr >= 0x4000 && addr <= 0x7FFF) {
        return ROM_banks[1][addr - 0x4000];
    } else if (addr < 0x4000) {
        return ROM_banks[0][addr];
    }

    
    if (addr < 0xFF00) {
        return mem[addr];
    } else {
        return io_get_mem(GLOBAL_TO_IO_ADDR(addr));
    }
}


void set_mem_16(uint16_t const loc, uint16_t const val) {
    set_mem(loc + 1, val >> 8);
    set_mem(loc, val & 0xFF);
}


uint16_t get_mem_16(uint16_t const loc) {
    return (get_mem(loc + 1) << 8) |
            get_mem(loc);
}


// Applies function to 8 bit value at the specified address
void mem_op(uint16_t addr, void (mem_op_fn)(uint8_t *)) {
    uint8_t temp = get_mem(addr); 
    mem_op_fn(&temp); /*  perform modifying op on temp */
    set_mem(addr, temp); /*  write new value temp back */
}




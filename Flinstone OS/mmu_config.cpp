
#include <stdio.h>
#include <stdlib.h>

int mmu_ram_kb = 4096;  // default
int mmu_storage_kb = 8192;

void configure_mmu(int ram_kb, int storage_kb) {
    mmu_ram_kb = ram_kb;
    mmu_storage_kb = storage_kb;
    printf("[MMU] Virtual memory configured: RAM=%dKB, Storage=%dKB\n", ram_kb, storage_kb);
}

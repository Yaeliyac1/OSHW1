#include "os.h"



void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn){
    int level;
    uint64_t curr_key;
    uint64_t * pointer_curr_level;
    pointer_curr_level = (uint64_t*)phys_to_virt(pt << 12);
    for(level = 4; level > 0 ; level++){
        curr_key = (vpn >> (9 * level)) & 0b111111111;
        if((pointer_curr_level[curr_key] & 1) == 0){
            if(ppn == NO_MAPPING){
                return;
            }
            pointer_curr_level[curr_key] = (alloc_page_frame() << 12) + 1; // new valid pte
        }
        pointer_curr_level = (uint64_t*)phys_to_virt(pointer_curr_level[curr_key] - 1);
    }
    /* level = 0 meaning we are in our last pt*/
    curr_key = vpn & 0b111111111;
    if(ppn == NO_MAPPING){
        pointer_curr_level[curr_key] = 0;
        return;
    }
    pointer_curr_level[curr_key] = (ppn << 12) + 1; // new valid pte
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn){
    int level;
    uint64_t curr_key;
    uint64_t * pointer_curr_level;
    pointer_curr_level = (uint64_t*)phys_to_virt(pt << 12);
    for(level = 4; level >= 0; level --){
        curr_key = (vpn >> (9 * level)) & 0b111111111;
        if(((pointer_curr_level[curr_key] & 1) == 0) || pointer_curr_level == NULL){
            return NO_MAPPING;
        }
        pointer_curr_level = (uint64_t*)phys_to_virt(pointer_curr_level[curr_key] - 1);
    }
    return pointer_curr_level[curr_key] >> 12;

}

    



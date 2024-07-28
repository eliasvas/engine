#include "entity.h"
// ref: https://bitsquid.blogspot.com/2014/08/building-data-oriented-entity-system.html

void nentity_manager_init(nEntityManager *em) {
    M_ZERO_STRUCT(em);
    em->arena = arena_alloc();

    nGenArrayChunk *new_chunk = push_array(em->arena, nGenArrayChunk, 1);
    new_chunk->generation = push_array(em->arena, u8, NGEN_CHUNK_COUNT);
    sll_queue_push(em->first, em->last, new_chunk);
}

void nentity_manager_destroy(nEntityManager *em){
    arena_clear(em->arena);
}

#define NENTITY_MIN_FREE_INDICES 1024

u32 nentity_index(nEntity e) {
    return (e & NENTITY_INDEX_MASK);
}

u32 nentity_generation(nEntity e) {
    return ((e >> NENTITY_INDEX_BITS) & NENTITY_GENERATION_MASK);
}

nEntity nentity_make(u32 idx, u32 generation) {
    return ((generation & NENTITY_GENERATION_MASK) << NENTITY_INDEX_BITS) | (idx & NENTITY_INDEX_MASK);
}

// TODO -- this code isn't very explanatory! fix dis
u32 nentity_manager_make_new_index(nEntityManager *em) {
    u32 idx;
    u32 running_index = 0;
    b32 index_created = 0;
    for (nGenArrayChunk *chunk = em->first; chunk != 0; chunk = chunk->next) {
        if (chunk->gen_count < NGEN_CHUNK_COUNT) {
            idx = running_index + chunk->gen_count;
            chunk->gen_count+=1;
            index_created = 1;
            break;
        }
        running_index += NGEN_CHUNK_COUNT;
    }
    // if index was not created, there wasn't enough space,
    // we should alloc another chunk and insert the index
    if (!index_created) {
        nGenArrayChunk *new_chunk = push_array(em->arena, nGenArrayChunk, 1);
        new_chunk->generation = push_array(em->arena, u8, NGEN_CHUNK_COUNT);
        new_chunk->gen_count = 1;
        idx = running_index;
        sll_queue_push(em->first, em->last, new_chunk);
    }
    return idx;
}

u8 *nentity_manager_get_generation_for_index(nEntityManager *em, u32 index) {
    u32 running_index = 0;
    for (nGenArrayChunk *chunk = em->first; chunk != 0; chunk = chunk->next) {
        if (index < running_index + chunk->gen_count) {
            return &(chunk->generation[index%NGEN_CHUNK_COUNT]);
        }
        running_index += NGEN_CHUNK_COUNT;
    }
    return 0;
}
u8 nentity_manager_get_generation_val_for_index(nEntityManager *em, u32 index) {
    u8 *gen = nentity_manager_get_generation_for_index(em, index);
    return *gen;
}
void nentity_manager_increment_generation_for_index(nEntityManager *em, u32 index) {
    u8 *gen = nentity_manager_get_generation_for_index(em, index);
    if (gen) {
        *(gen)+=1;
    }
}


nEntity nentity_create(nEntityManager *em) {
    u32 idx;
    if (em->free_indices_count > NENTITY_MIN_FREE_INDICES) {
        nFreeIndexNode *n = sll_queue_pop(em->free_indices_first, em->free_indices_last);
        idx = n->index;
    }else {
        idx = nentity_manager_make_new_index(em);
    }
    return nentity_make(idx, nentity_manager_get_generation_val_for_index(em, idx));
}

b32 nentity_alive(nEntityManager *em, nEntity e) {
    u8 *generation = nentity_manager_get_generation_for_index(em, nentity_index(e));
    return ((generation != 0) && (*generation == nentity_generation(e)));
}

void nentity_destroy(nEntityManager *em, nEntity e) {
    u32 idx = nentity_index(e);
    nentity_manager_increment_generation_for_index(em, idx);

    // push current index in free_index list for reuse
    nFreeIndexNode *free_idx = push_array(em->arena, nFreeIndexNode, 1); 
    free_idx->index = idx;
    sll_queue_push(em->free_indices_first, em->free_indices_last, free_idx);
    em->free_indices_count+=1;
}
#include "comp.h"
#include "game_state.h"
#include "tileset4922.inl"

// TODO -- COLLIDER_VISUALIZATION should probably be an option in GameState
//#define COLLIDER_VISUALIZATION 1

nHealthComponent nhealth_component_make(s32 max_health) {
    nHealthComponent n = {
        .max_hlt = max_health,
        .hlt = max_health,
    };
    return n;
}

void nhealth_component_enc(nHealthComponent *hc) {
    hc->hlt = minimum(hc->hlt+1, hc->max_hlt);
}

void nhealth_component_dec(nHealthComponent *hc) {
    hc->hlt = maximum(hc->hlt-1, 0);
}

b32 nhealth_component_alive(nHealthComponent *hc) {
    return (hc->hlt > 0);
}

nAIComponent nai_component_default(void) {
    nAIComponent ai = {
        .state = 0,
        .timestamp= get_current_timestamp_sec(),
        .invinsibility_sec = 0.0,
        .dead = 0,
    };
    return ai;
}

void ai_component_enemy_update(nEntityMgr *em, GameState *gs, nEntityID enemy) {
    nEntityID player = gs->player;
    nPhysicsBody *b_player = NENTITY_MANAGER_GET_COMPONENT(em, player, nPhysicsBody);
    nPhysicsBody *b_enemy = NENTITY_MANAGER_GET_COMPONENT(em, enemy, nPhysicsBody);
    vec2 dist = vec2_sub(b_player->position, b_enemy->position);
    f32 enemy_speed = 20;
    f32 dt = nglobal_state_get_dt_sec();
    // TODO -- maybe this 5 should be part of nAIComponent or some enemy logic
    if (vec2_len(dist) < 5) {
        dist = vec2_norm(dist);
        b_enemy->velocity = vec2_add(vec2_multf(dist, enemy_speed*dt), b_enemy->velocity);
    }
}

void ai_component_player_update(nEntityMgr *em, nEntityID player) {
    nPhysicsBody *b = NENTITY_MANAGER_GET_COMPONENT(em, player, nPhysicsBody);
    nSprite *s = NENTITY_MANAGER_GET_COMPONENT(em, player, nSprite);
    f32 dt = nglobal_state_get_dt_sec();
    f32 player_speed = 50.0;
    b32 face_right = 1;
    b32 hmov = 0;
    if (ninput_key_down(get_nim(),NKEY_SCANCODE_RIGHT)) { b->velocity.x+=player_speed*dt; face_right = 1; hmov=1; }
    if (ninput_key_down(get_nim(),NKEY_SCANCODE_LEFT))  { b->velocity.x-=player_speed*dt; face_right = 0; hmov=1; }
    if (ninput_key_down(get_nim(),NKEY_SCANCODE_UP))    { b->velocity.y-=player_speed*dt; }
    if (ninput_key_down(get_nim(),NKEY_SCANCODE_DOWN))  { b->velocity.y+=player_speed*dt; }
    if (hmov) { s->vflip = !face_right; }
    if (equalf(b->velocity.x,0.0,0.05) && equalf(b->velocity.y,0.0,0.05)){
        s->repeat=0;
    } else {
        if (b->inv_mass != 0) {
            s->repeat=1;
        }
    }
    nAIComponent *ai = NENTITY_MANAGER_GET_COMPONENT(get_em(), player, nAIComponent);
    ai->invinsibility_sec = maximum(0.0, ai->invinsibility_sec - dt);
}

void game_ai_system(nEntityMgr *em, void *ctx) {
    GameState *gs = (GameState*)ctx;
    // do default game AI

    // branch of to logic for certain AI kinds (player/enemy/projectile)
    for (s64 i = 0; i < em->comp_array_len; i+=1) {
        nEntityID entity = NENTITY_MANAGER_GET_ENTITY_FOR_INDEX(get_em(), i);
        nEntityTag tag = *(NENTITY_MANAGER_GET_COMPONENT(em, entity, nEntityTag));
        switch (tag) {
            case NENTITY_TAG_PLAYER: 
                ai_component_player_update(em, entity);
                break;
            case NENTITY_TAG_ENEMY: 
                ai_component_enemy_update(em, gs, entity);
                break;
            // case NENTITY_TAG_PROJECTILE: 
            // case NENTITY_TAG_DOOR: 
            default:
                break;
        } 
    }
}


void render_sprites_system(nEntityMgr *em, void *ctx) {
    GameState *gs = (GameState*)ctx;

    nbatch2d_rend_begin(&gs->batch_rend, get_nwin());
    mat4 view = ndungeon_cam_get_view_mat(&gs->dcam);
    nbatch2d_rend_set_view_mat(&gs->batch_rend, view);
    for (s64 i = em->comp_array_len; i>=0; i-=1) {
        nEntityID entity = NENTITY_MANAGER_GET_ENTITY_FOR_INDEX(get_em(), i);

        // render the sprites with physics body
        if (NENTITY_MANAGER_HAS_COMPONENT(em, entity, nSprite) && NENTITY_MANAGER_HAS_COMPONENT(em, entity, nPhysicsBody)) {
            nSprite *s = NENTITY_MANAGER_GET_COMPONENT(em, entity, nSprite);
            nPhysicsBody *b = NENTITY_MANAGER_GET_COMPONENT(em, entity, nPhysicsBody);
            nsprite_update(s, nglobal_state_get_dt_sec());
            vec4 tc = nsprite_get_current_tc(s);
            nBatch2DQuad q = {0};
            q.color = s->color;
            // we convert our collider to a bounding box (which is used for rendering dimensions)
            q.tc = tc;
            vec2 sprite_dim = (b->c_kind == NCOLLIDER_KIND_CIRCLE) ? v2(b->radius, b->radius) : b->hdim;
            sprite_dim = vec2_multf(sprite_dim, 2);
            q.pos.x = b->position.x - sprite_dim.x/2.0;
            q.pos.y = b->position.y - sprite_dim.y/2.0;
            q.dim.x = sprite_dim.x;
            q.dim.y = sprite_dim.y;
            q.angle_rad = 0;
            nbatch2d_rend_add_quad(&gs->batch_rend, q, &gs->atlas);
            // if entity has health, draw that as well
            if (NENTITY_MANAGER_HAS_COMPONENT(em,entity, nHealthComponent)) {
                nHealthComponent *h = NENTITY_MANAGER_GET_COMPONENT(em, entity, nHealthComponent);
                u32 heart_count = h->hlt;
                f32 heart_sprite_dim = (sprite_dim.x/2.0);
                f32 total_width = heart_sprite_dim * heart_count;
                f32 start_y = q.pos.y - heart_sprite_dim;
                f32 start_x = b->position.x - total_width/2.0;
                for (u32 heart = 0; heart < heart_count; heart+=1) {
                    nBatch2DQuad q = {0};
                    q.color = s->color;
                    q.tc = TILESET_HEART_TILE;
                    q.pos.x = start_x + heart*heart_sprite_dim;
                    q.pos.y = start_y;
                    q.dim.x = heart_sprite_dim;
                    q.dim.y = heart_sprite_dim;
                    q.angle_rad = 0;
                    nbatch2d_rend_add_quad(&gs->batch_rend, q, &gs->atlas);
                }
            }
        }

#if COLLIDER_VISUALIZATION
        // render the stuff
        if (NENTITY_MANAGER_HAS_COMPONENT(em, entity, nSprite) && NENTITY_MANAGER_HAS_COMPONENT(em, entity, nPhysicsBody)) {
            if (((nPhysicsBody*)NENTITY_MANAGER_GET_COMPONENT(em, entity, nPhysicsBody))->collider_off)continue;
            nSprite *s = NENTITY_MANAGER_GET_COMPONENT(em, entity, nSprite);
            nPhysicsBody *b = NENTITY_MANAGER_GET_COMPONENT(em, entity, nPhysicsBody);
            nsprite_update(s, nglobal_state_get_dt_sec());
            vec4 tc = nsprite_get_current_tc(s);
            nBatch2DQuad q = {0};
            q.color = v4(1,0,0,0.3);
            q.tc = (b->c_kind == NCOLLIDER_KIND_CIRCLE) ? TILESET_CIRCLE_TILE : TILESET_SOLID_TILE;
            vec2 sprite_dim = (b->c_kind == NCOLLIDER_KIND_CIRCLE) ? v2(b->radius, b->radius) : b->hdim;
            sprite_dim = vec2_multf(sprite_dim, 2);
            q.pos.x = b->position.x - sprite_dim.x/2.0;
            q.pos.y = b->position.y - sprite_dim.y/2.0;
            q.dim.x = sprite_dim.x;
            q.dim.y = sprite_dim.y;
            q.angle_rad = 0;
            nbatch2d_rend_add_quad(&gs->batch_rend, q, &gs->atlas);
        }
#endif
    }
    nbatch2d_rend_end(&gs->batch_rend);
}

void resolve_collision_events(nEntityMgr *em, void *ctx) {
    GameState *gs = (GameState*)ctx;

    for (nEntityEventNode *en = em->event_mgr.first; en != 0; en = en->next) {
        nEntityTag tag_a = *(NENTITY_MANAGER_GET_COMPONENT(em, en->e.entity_a, nEntityTag));
        nEntityTag tag_b = *(NENTITY_MANAGER_GET_COMPONENT(em, en->e.entity_b, nEntityTag));
        // swap entity places for player to be entity_a
        if (tag_b == NENTITY_TAG_PLAYER && tag_a == NENTITY_TAG_ENEMY) {
            nEntityID temp = en->e.entity_a;
            en->e.entity_a = en->e.entity_b;
            en->e.entity_b = temp;
            tag_a = *(NENTITY_MANAGER_GET_COMPONENT(em, en->e.entity_a, nEntityTag));
            tag_b = *(NENTITY_MANAGER_GET_COMPONENT(em, en->e.entity_b, nEntityTag));
        }
        // player collides with enemy
        if (tag_a == NENTITY_TAG_PLAYER && tag_b == NENTITY_TAG_ENEMY) {
            nHealthComponent *h = NENTITY_MANAGER_GET_COMPONENT(get_em(), en->e.entity_a, nHealthComponent);
            nAIComponent *ai = NENTITY_MANAGER_GET_COMPONENT(get_em(), en->e.entity_a, nAIComponent);
            if (ai->invinsibility_sec == 0.0) {
                ai->invinsibility_sec = 200.0;
                nhealth_component_dec(h);
                if (!nhealth_component_alive(h)) {
                    // set sprite so that death animation will play
                    nSprite *s = NENTITY_MANAGER_GET_COMPONENT(get_em(), en->e.entity_a, nSprite);
                    s->frame_count = 6;
                    s->repeat = 0;
                    // make collider static
                    nPhysicsBody *b = NENTITY_MANAGER_GET_COMPONENT(get_em(), en->e.entity_a, nPhysicsBody);
                    b->mass = F32_MAX;
                    b->inv_mass = 0;
                    b->velocity = v2(0,0);
                }
            }
      }
    }
}

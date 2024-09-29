#define STB_IMAGE_IMPLEMENTATION
#include "game_state.h"
#include "map.h"
#include "engine.h"
#include "comp.h"
#include "tileset4922.inl"
// TODO -- VERY VERY VERY important, please please please reamove as many singletons as possible, pass 'parents' as *self

GameState gs = {0};

GameState *get_ggs() {
    return &gs;
}



oglImage game_load_rgba_image_from_disk(const char *path) {
    oglImage img;
    s32 w,h,comp;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* image = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);
    if(stbi_failure_reason()) {
        NLOG_ERR("Failed reading image: %s\n", stbi_failure_reason());
    }
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    assert(ogl_image_init(&img, image, w, h, OGL_IMAGE_FORMAT_RGBA8U));
    stbi_image_free(image);
    return img;
}


void game_state_init_images() {
    gs.atlas = game_load_rgba_image_from_disk("assets/tileset4922.png");
    u32 white = 0xFFFF;
    ogl_image_init(&gs.white, (u8*)(&white), 1, 1, OGL_IMAGE_FORMAT_R8U);
}

extern void nphysics_world_update_func(nEntityMgr *em);

void game_state_init() {
    game_state_status_set(GAME_STATUS_STARTUP);
    game_state_init_images();

    NENTITY_MANAGER_INIT(get_em());
    NENTITY_MANAGER_COMPONENT_REGISTER(get_em(), nEntityTag);
    NENTITY_MANAGER_COMPONENT_REGISTER(get_em(), nPhysicsBody);
    NENTITY_MANAGER_COMPONENT_REGISTER(get_em(), nSprite);
    NENTITY_MANAGER_COMPONENT_REGISTER(get_em(), nHealthComponent);
    NENTITY_MANAGER_COMPONENT_REGISTER(get_em(), nAIComponent);
    NENTITY_MANAGER_ADD_SYSTEM(get_em(), nphysics_world_update_func, 1);
    NENTITY_MANAGER_ADD_SYSTEM(get_em(), resolve_collision_events, 2);
    NENTITY_MANAGER_ADD_SYSTEM(get_em(), game_ai_system, 3);
    NENTITY_MANAGER_ADD_SYSTEM(get_em(), render_sprites_system, 4);

    game_state_status_set(GAME_STATUS_START_MENU);
}

void game_state_deinit() {
    // TBA
}

void game_state_update_and_render() {
    vec2 screen_coords =  ndungeon_cam_screen_to_world(&gs.dcam, ninput_get_mouse_pos(get_nim()));
    do_game_gui();
    if (ninput_key_pressed(get_nim(), NKEY_SCANCODE_ESCAPE)) {game_state_status_set(GAME_STATUS_START_MENU);}

    if (game_state_status_match(GAME_STATUS_RUNNING)) {
        nPhysicsBody *b = NENTITY_MANAGER_GET_COMPONENT(get_em(), gs.player, nPhysicsBody);
        vec2 player_pos = v2(b->position.x, b->position.y);
        // NLOG_ERR("camera coords: %f %f", gs.dcam.pos.x, gs.dcam.pos.y);
        // NLOG_ERR("world coords: %f %f", screen_coords.x, screen_coords.y);
        //NLOG_ERR("player pos : %f %f", player_pos.x, player_pos.y);


        nScrollAmount scroll_y = ninput_get_scroll_amount_delta(get_nim());
        if (scroll_y) { gs.dcam.zoom += signof(scroll_y) * 4; }
        ndungeon_cam_update(&gs.dcam, v2(player_pos.x, player_pos.y));
        // ----
        nem_update(get_em());
    }

    if (ninput_key_pressed(get_nim(), NKEY_SCANCODE_SPACE)) {
        game_state_status_set(game_state_status_match(GAME_STATUS_PAUSED) ? GAME_STATUS_RUNNING : GAME_STATUS_PAUSED);
    }
}

void game_state_status_set(GameStatus status) {
    gs.status = status;
}

b32  game_state_status_match(GameStatus status) {
    return (gs.status == status);
}

void game_state_generate_new_level() {
    NENTITY_MANAGER_CLEAR(get_em());
    ndungeon_cam_set(&gs.dcam, v2(0,0), v2(10,10), 50);
    // init player entity
    gs.player = nem_make(get_em());
    NENTITY_MANAGER_ADD_COMPONENT(get_em(), gs.player, nPhysicsBody);
    NENTITY_MANAGER_ADD_COMPONENT(get_em(), gs.player, nSprite);
    NENTITY_MANAGER_ADD_COMPONENT(get_em(), gs.player, nEntityTag); // Maybe tag should be instantiated in nem_make(em)
    NENTITY_MANAGER_ADD_COMPONENT(get_em(), gs.player, nHealthComponent);
    NENTITY_MANAGER_ADD_COMPONENT(get_em(), gs.player, nAIComponent);
    *NENTITY_MANAGER_GET_COMPONENT(get_em(), gs.player, nSprite) = nsprite_make(TILESET_ANIM_PLAYER_TILE, 5, 2, v4(1,0.3,0.3,1.0));
    *NENTITY_MANAGER_GET_COMPONENT(get_em(), gs.player, nHealthComponent) = nhealth_component_make(3); 
    *NENTITY_MANAGER_GET_COMPONENT(get_em(), gs.player, nAIComponent) = nai_component_default();
    nPhysicsBody *b = NENTITY_MANAGER_GET_COMPONENT(get_em(), gs.player, nPhysicsBody);
    //*b = nphysics_body_aabb(v2(10,10), 200*gen_rand01());
    *b = nphysics_body_circle(0.5, 20);
    b->position = v2(0,0);
    b->gravity_scale = 0;
    nEntityTag *player_tag = NENTITY_MANAGER_GET_COMPONENT(get_em(), gs.player, nEntityTag);
    *player_tag = NENTITY_TAG_PLAYER;

    // generate a NEW map
    nMap map = {0};
    nmap_create(&map, 32, 32);
}
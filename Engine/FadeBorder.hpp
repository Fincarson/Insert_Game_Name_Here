//
// Created by Vincent Jefferson on 6/9/2025.
//

#ifndef FADEBORDER_HPP
#define FADEBORDER_HPP

#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

// draws a black→transparent gradient strip of width `thickness` around the edges
inline void draw_fading_border(ALLEGRO_DISPLAY* disp, int thickness) {
    int W = al_get_display_width(disp);
    int H = al_get_display_height(disp);

    // single shared decl: position + 4-byte RGBA
    static ALLEGRO_VERTEX_DECL* decl = nullptr;
    if (!decl) {
        static const ALLEGRO_VERTEX_ELEMENT elems[] = {
            { ALLEGRO_PRIM_POSITION,   0,                               ALLEGRO_PRIM_FLOAT_2 },
            { ALLEGRO_PRIM_COLOR_ATTR, offsetof(ALLEGRO_VERTEX, color), ALLEGRO_PRIM_UBYTE_4 },
            { 0 }
        };
        decl = al_create_vertex_decl(elems, 0);
    }

    auto drawEdge = [&](float x1, float y1, float x2, float y2,
                        float x3, float y3, float x4, float y4)
    {
        ALLEGRO_VERTEX v[4];
        // fully opaque
        v[0].x = x1; v[0].y = y1; v[0].z = 0; v[0].color = al_map_rgba(0,0,0,255);
        // fully transparent
        v[1].x = x2; v[1].y = y2; v[1].z = 0; v[1].color = al_map_rgba(0,0,0,  0);
        // fully opaque
        v[2].x = x3; v[2].y = y3; v[2].z = 0; v[2].color = al_map_rgba(0,0,0,255);
        // fully transparent
        v[3].x = x4; v[3].y = y4; v[3].z = 0; v[3].color = al_map_rgba(0,0,0,  0);

        al_draw_prim(v, decl, nullptr, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
    };

    // left
    drawEdge(  0,   0,  thickness,   0,
               0,   H,  thickness,   H);
    // right
    drawEdge(W-thickness,   0,  W,   0,
             W-thickness,   H,  W,   H);
    // top
    drawEdge(  0,   0,  W,   0,
               0,   thickness,  W,   thickness);
    // bottom
    drawEdge(  0, H-thickness,  W, H-thickness,
               0,   H,         W,   H);
}


#endif //FADEBORDER_HPP

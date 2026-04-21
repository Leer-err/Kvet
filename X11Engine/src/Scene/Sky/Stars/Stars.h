#pragma once

#include "Buffer.h"
#include "GraphicsPipeline.h"
// #include "Mesh.h"

class Stars {
   public:
    Stars();

    void draw();

   private:
    // Mesh screen_plane;
    Graphics::Buffer quad_vertices;
    Graphics::Buffer quad_indices;
    Graphics::GraphicsPipeline pipeline;
    Graphics::Buffer camera_parameters_buffer;
    Graphics::Buffer star_parameters_buffer;

    float star_density;
    float blinking_speed;
    float blink_strength;
};
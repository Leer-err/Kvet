#pragma once

// #include "Mesh.h"

class Stars {
   public:
    Stars();

    void draw();

   private:
    float star_density;
    float blinking_speed;
    float blink_strength;
};
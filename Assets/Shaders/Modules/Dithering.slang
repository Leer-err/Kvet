module Dithering;

static const float2x2 bayer_2x2 = { {0, 2}, {3, 1} };

static const float4x4 bayer_4x4 = { {0, 8, 2, 10},
                                    {12, 4, 14, 6},
                                    { 3, 11, 1, 9},
                                    {15, 7, 13, 5} };

static const float bayer_8x8[8][8] = { { 0, 48, 12, 60,  3, 51, 15, 63},
                                       {32, 16, 44, 28, 35, 19, 47, 31},
                                       { 8, 56,  4, 52, 11, 59,  7, 55},
                                       {40, 24, 36, 20, 43, 27, 39, 23},
                                       { 2, 50, 14, 62,  1, 49, 13, 61},
                                       {34, 18, 46, 30, 33, 17, 45, 29},
                                       {10, 58,  6, 54,  9, 57,  5, 53},
                                       {42, 26, 38, 22, 41, 25, 37, 21} };

public float3 dithering2x2(float3 color, float spread, uint2 pixel_coordinates){
    static const uint SIZE = 2;

    int2 threshold_coordinates = pixel_coordinates % int2(SIZE, SIZE);

    float noise = bayer_2x2[threshold_coordinates.x][threshold_coordinates.y] / (SIZE * SIZE) - 0.5;

    return color + noise * spread;
}

public float3 dithering4x4(float3 color, float spread, uint2 pixel_coordinates){
    static const uint SIZE = 4;

    int2 threshold_coordinates = pixel_coordinates % int2(SIZE, SIZE);

    float noise = bayer_4x4[threshold_coordinates.x][threshold_coordinates.y] / (SIZE * SIZE) - 0.5;

    return color + noise * spread;
}

public float3 dithering8x8(float3 color, float spread, uint2 pixel_coordinates){
    static const uint SIZE = 8;

    int2 threshold_coordinates = pixel_coordinates % int2(SIZE, SIZE);

    float noise = bayer_8x8[threshold_coordinates.x][threshold_coordinates.y] / (SIZE * SIZE) - 0.5;

    return color + noise * spread;
}
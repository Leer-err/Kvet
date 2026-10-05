module Math;

public float remap(float value, float in_min, float in_max, float out_min, float out_max)
{
    float t = (value - in_min) / (in_max - in_min);
    
    return lerp(out_min, out_max, t);
}
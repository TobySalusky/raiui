uniform float hue;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    float wPercent = fragCoord.x / iResolution.x;
    float hPercent = fragCoord.y / iResolution.y;
    fragColor = vec4(hsv2rgb(vec3(hue, wPercent, hPercent)), 1.0);
}
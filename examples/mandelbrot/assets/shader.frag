#version 410

uniform vec2 area_w;
uniform vec2 area_h;
uniform uint max_iterations;

out vec4 pixel_color;

void main()
{
    vec3 color_map[17];
    color_map[0] = vec3(0.0,  0.0,  0.0);
    color_map[1] = vec3(0.26, 0.18, 0.06);
    color_map[2] = vec3(0.1,  0.03, 0.1);
    color_map[3] = vec3(0.04, 0.0,  0.18);
    color_map[4] = vec3(0.02, 0.02, 0.29);
    color_map[5] = vec3(0.0,  0.03, 0.39);
    color_map[6] = vec3(0.05, 0.17, 0.54);
    color_map[7] = vec3(0.09, 0.32, 0.69);
    color_map[8] = vec3(0.22, 0.49, 0.82);
    color_map[9] = vec3(0.52, 0.71, 0.9);
    color_map[10] = vec3(0.82, 0.92, 0.97);
    color_map[11] = vec3(0.94, 0.91, 0.75);
    color_map[12] = vec3(0.97, 0.79, 0.37);
    color_map[13] = vec3(1.0,  0.67, 0.0);
    color_map[14] = vec3(0.8,  0.5,  0.0);
    color_map[15] = vec3(0.6,  0.34, 0.0);
    color_map[16] = vec3(0.41, 0.2,  0.01);

    float rect_width = 800.0;
    float rect_height = 600.0;

    vec2 C = vec2(gl_FragCoord.x * (area_w.y - area_w.x) / rect_width  + area_w.x,
                        gl_FragCoord.y * (area_h.y - area_h.x) / rect_height + area_h.x);
    vec2 Z = vec2(0.0);
    uint iteration = 0u;

    while (iteration < max_iterations)
    {
        float x = Z.x * Z.x - Z.y * Z.y + C.x;
        float y = 2.0 * Z.x * Z.y       + C.y;

        if (x * x + y * y > 4.0)
            break;

        Z.x = x;
        Z.y = y;

        ++iteration;
    }

    uint row_index = (iteration * 100u / max_iterations % 17u);
    pixel_color = vec4((iteration == max_iterations ? vec3(0.0) : color_map[row_index]), 1.0);
}
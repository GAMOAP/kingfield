uniform vec3 u_color ;    // Stroke color
uniform float u_line_size ;    //Stroke offset size

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

// Get the back area
float getBackArea()
{
    vec4 color_up;
    vec4 color_down;
    vec4 color_left;
    vec4 color_right;
    vec4 color_up_left;
    vec4 color_up_right;
    vec4 color_down_left;
    vec4 color_down_right;
    float total = 0.0;
    
    color_up = texture2D(CC_Texture0, v_texCoord + vec2(0, u_line_size));
    color_down = texture2D(CC_Texture0, v_texCoord - vec2(0, u_line_size));
    color_left = texture2D(CC_Texture0, v_texCoord - vec2(u_line_size, 0));
    color_right = texture2D(CC_Texture0, v_texCoord + vec2(u_line_size, 0));
    color_up_left = texture2D(CC_Texture0, v_texCoord + vec2(u_line_size, -u_line_size));
    color_up_right = texture2D(CC_Texture0, v_texCoord + vec2(u_line_size, u_line_size));
    color_down_left = texture2D(CC_Texture0, v_texCoord + vec2(-u_line_size, -u_line_size));
    color_down_right = texture2D(CC_Texture0, v_texCoord + vec2(-u_line_size, u_line_size));
    total = color_right.a + color_left.a + color_down.a + color_up.a + color_up_left.a + color_up_right.a + color_down_left.a + color_down_right.a;
    return clamp(total, 0.0, 1.0);
}

void main()
{
    // Get the normal color value
    vec4 fragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    float isBack = getBackArea();
    
    // normal rgb + normal transparent area * back color * stroke color , back color
    gl_FragColor = vec4(fragColor.rgb + (1.0 - fragColor.a)* u_color * isBack, isBack);
}

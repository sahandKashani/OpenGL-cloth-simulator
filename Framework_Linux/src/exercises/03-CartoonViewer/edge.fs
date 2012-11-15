uniform sampler2D texture; //the depth texture that need to be post processed with the sobel operator

uniform float dx; //use this uniform to move 1 pixel in x
uniform float dy; //use this uniform to move 1 pixel in y

void main() {
    //
    // calculate sobel edges
    //               -1  0  1
    // x derivative: -2  0  2
    //               -1  0  1
    //
    //                1  2  1
    // y derivative:  0  0  0
    //               -1 -2 -1
    //
    
    // create x-axis derivative kernel
    vec3 gx_col_1 = vec3(-1, -2, -1);
    vec3 gx_col_2 = vec3(0, 0, 0);
    vec3 gx_col_3 = vec3(1, 2, 1);
    mat3 gx = mat3(gx_col_1, gx_col_2, gx_col_3);
    
    // create y-axis derivative kernel
    vec3 gy_col_1 = vec3(1, 0, -1);
    vec3 gy_col_2 = vec3(2, 0, -2);
    vec3 gy_col_3 = vec3(1, 0, -1);
    mat3 gy = mat3(gy_col_1, gy_col_2, gy_col_3);
    
    // declare gradient vector at point (i, j)
    vec2 gradient = vec2(0.0, 0.0);
    
    // increment in the texture coordinates.
    vec2 increment = vec2(dx, dy);
    // offset is going to be (m-1, n-1) to walk around the center texture coordinate.
    vec2 offset = vec2(0.0, 0.0);
    // exactly what texture coordinates to look up based on offset.
    vec2 lookup_texture_position = vec2(0.0, 0.0);
    
    for (int m = 0; m < 3; m += 1) {
        for (int n = 0; n < 3; n += 1) {
        
            // calculate offset and use it to find appropriate texture coordinates.
            offset = vec2(m - 1, n - 1);
            lookup_texture_position = gl_TexCoord[0].xy + (increment * offset);
            
            // apply convolution formula. we use "rg" to extract the first 2 elements of the vec4 returned by texture2D, because we want to have a float vector to multipy with our kernel values.
            gradient += vec2(gx[m][n], gy[m][n]) * texture2D(texture, lookup_texture_position).rg;
        }
    }
    
    // Calculate "magnitude" of the rgb channels of fragment (i, j).
    // Note that length(gradient) is the actual magnitude, but we are asked to use a custom function to scale its value.
    float rgb_magnitude = 1.0 - (10.0 * length(gradient));
    
    // set rgb channels to caluclated magnitude.
    vec3 color = vec3(rgb_magnitude, rgb_magnitude, rgb_magnitude);
    gl_FragColor = vec4(color, 1.0);
    
}
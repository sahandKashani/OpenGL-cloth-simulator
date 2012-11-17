varying vec3 normal;

uniform sampler2D texture; //the 1D texture that store the discrete values for the shading

uniform int useTexture;
uniform sampler2D textureDiffuse;
uniform vec3 diffuseColor;

void main() {

    vec3 color = ((useTexture > 0) ? diffuseColor * texture2D(textureDiffuse, gl_TexCoord[0].xy).xyz : diffuseColor);
    
    vec3 L = vec3(0.0, 0.0, 1.0);
    vec3 N = normalize(normal);
    
    // continuous diffuse shading coefficient used to lookup in the 1D texture
    float diffuseCoefficient = max(0.0, dot(N, L));
    
    // discrete shading coefficient used for the actual color
    // I can't understand why a 1D texture is stored in a 2D texture here.
    // But since it's a 2D texture, we assume that the second dimension is empty.
    // That is why the second component of lookup_texture_position is 0.0.
    
    vec2 lookup_texture_position = vec2(diffuseCoefficient, 0.0);
    vec3 shadingCoefficient = texture2D(texture, lookup_texture_position).xyz;
    color = shadingCoefficient * color;
    
    gl_FragColor = vec4(color, 1.0);
    
}

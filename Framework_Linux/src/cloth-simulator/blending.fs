uniform sampler2D texture1; // the toon-shading texture
uniform sampler2D texture2; // the edge texture


void main() {
    vec4 cartoonColor = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 edgeColor = texture2D(texture2, gl_TexCoord[0].xy);
    
    // multiply the cartoonColor vector of colors with the edgeColor vector of colors to get the final color to show.
    vec4 final_color = cartoonColor * edgeColor;
    gl_FragColor = final_color;
    
}
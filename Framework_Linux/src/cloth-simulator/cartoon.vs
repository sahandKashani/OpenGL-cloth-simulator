uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;

varying vec3 normal;

void main() {
    // transform normal to camera coordinates
    normal = (worldcamera * modelworld * vec4(gl_Normal, 0.0)).xyz;
    
    // transform vertex to camera coordinates
    vec3 vertex = vec3(worldcamera * modelworld * gl_Vertex);
    
    // get texture coordinate
    gl_TexCoord[0]  = gl_MultiTexCoord0;
    
    // project the point into the camera
    gl_Position = projection * vec4(vertex, 1.0);
    
}

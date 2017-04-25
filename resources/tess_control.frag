#version 410

layout(vertices = 4) out; //Four control points per patch
in vec3[] vPosition;
out vec3 tcPosition[];

void main() {
    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    if(gl_InvocationID == 0) { //Only have to define tessellation once per vertex patch
        
        //Determines number of subdivisions per line
        gl_TessLevelOuter[10] = 1; //Tesselation in y for control points given (ie # of lines added)
        
        gl_TessLevelOuter[1] = 1; //Tesselation in x for line generated (only need 1 line for x since we are dividing based upon the y parameter)
    }
}
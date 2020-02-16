#version 130

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform float scale_factor;
uniform vec3 theta;

void main() {
    color = vColor;
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );
    float radius = 0.2;
    float pi = 3.1415;
    float translate_x = -1.0*(radius*pi) + (radius*pi*(-theta.z/360.0));
    mat4 scale_m = mat4( scale_factor,  0.0,  0.0, 0.0,
		                    0.0,  scale_factor,  0.0, 0.0,
                		    0.0,  0.0,  scale_factor, 0.0,
		                    0.0,  0.0,  0.0, 1.0 );
    
    mat4 translate = mat4( 1.0,  0.0,  0.0, 0.0,
		                    0.0,  1.0,  0.0, 0.0,
                		    0.0,  0.0,  1.0, 0.0,
		                    translate_x, 0.0,  0.0, 1.0 );
                            // Remeber: thse matrices are column-major
                            
    mat4 rx = mat4( 1.0,  0.0,  0.0, 0.0,
		    0.0,  c.x,  s.x, 0.0,
		    0.0, -s.x,  c.x, 0.0,
		    0.0,  0.0,  0.0, 1.0 );
    

    mat4 ry = mat4( c.y, 0.0, -s.y, 0.0,
		    0.0, 1.0,  0.0, 0.0,
		    s.y, 0.0,  c.y, 0.0,
		    0.0, 0.0,  0.0, 1.0 );

    // Workaround for bug in ATI driver
    ry[1][0] = 0.0;
    ry[1][1] = 1.0;

    mat4 rz = mat4( c.z, s.z, 0.0, 0.0,
		    -s.z,  c.z, 0.0, 0.0,
		    0.0,  0.0, 1.0, 0.0,
		    0.0,  0.0, 0.0, 1.0 );

    // Workaround for bug in ATI driver
    rz[2][2] = 1.0;
    color = vColor;
    if(scale_factor == 1.0){
        gl_Position = rz * ry * rx * scale_m * vPosition;

    }
    else{
        gl_Position = translate* rz * ry * rx * scale_m * vPosition;
    }
}

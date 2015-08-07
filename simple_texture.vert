# version 330
in vec4 vPosition ;
in vec2 vcoordText ;

uniform mat4 modelView ;
uniform mat4 projectionMatrix ;

out vec2 fTextCoord;

void main (){
    vec4 eyePosition = modelView * vPosition ;
    gl_Position = projectionMatrix * eyePosition ;

    fTextCoord = vcoordText;
}

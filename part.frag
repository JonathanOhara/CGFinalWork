# version 330
uniform sampler2D colorTexture;
in vec2 fTextCoord;

out vec4 frag_color;

void main ( void ) {
    frag_color = texture2D(colorTexture, fTextCoord);
}

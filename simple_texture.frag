# version 330
uniform sampler2D colorTexture;
in vec2 fTextCoord;

out vec4 frag_color;

void main ( void ) {
    frag_color = texture( colorTexture , fTextCoord );
    //frag_color = texture2D( colorTexture , fTextCoord );
    //frag_color = vec4( 0.0, 0.0, 0.0, 1.0);
}

uniform mat4 LProjectionMatrix;
uniform mat4 LModelViewMatrix;

attribute vec2 LVertexPos2D;
attribute vec2 LTexCoord;

varying vec2 texCoord;
varying vec3 iPosition;

void main()
{
	texCoord = LTexCoord;
	iPosition = gl_Vertex.xyz;
	
	gl_Position = LProjectionMatrix * LModelViewMatrix * vec4( LVertexPos2D.x, LVertexPos2D.y, 0.0, 1.0 );
}
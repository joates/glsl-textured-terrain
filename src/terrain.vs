varying vec3 iPosition;

void main()
{
	iPosition = gl_Vertex.xyz;
	gl_Position = gl_Vertex;
}
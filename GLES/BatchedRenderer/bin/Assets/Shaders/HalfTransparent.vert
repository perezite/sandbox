attribute vec2 a_vPosition;										
attribute vec4 a_vColor;											
varying vec4 v_vColor;		
										
void main()														
{																	
	gl_Position = vec4(a_vPosition.x, a_vPosition.y, 0 , 1 );		
	v_vColor = a_vColor;											
}
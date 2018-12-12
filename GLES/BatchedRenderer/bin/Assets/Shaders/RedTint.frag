#version 100									
precision mediump float;	
					
varying vec4 v_vColor;
		 					
void main()									
{												
	gl_FragColor = vec4(v_vColor.r, v_vColor.g, v_vColor.b, 1) * vec4(1, 0.2, 0.2, 1);			
}												

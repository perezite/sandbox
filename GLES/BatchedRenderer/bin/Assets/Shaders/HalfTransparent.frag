#version 100									
precision mediump float;	
					
varying vec4 v_vColor;
		 					
void main()									
{												
	gl_FragColor = vec4(v_vColor.rgb, 0.1);			
}												

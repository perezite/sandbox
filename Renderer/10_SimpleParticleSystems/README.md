# Content
Some maintenance work: Matrices are now column major and therefore compatible with GLSL shaders. 
This way, we save a costly matrix transposition in the renderer.
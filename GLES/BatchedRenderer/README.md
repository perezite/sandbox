# Content
A gl/gles renderer with indexed batching
It can do the following:
	- Supports rendering with multiple shaders
	- Renders up to USHORT_MAX indices per draw batch
	- Applies auto-batching to drawables outside of a draw batch
	- Supports shaders
	- Supports dirty transforms (Transforms are only computed, if it actually changed)
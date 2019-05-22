The following particle parameters are currently relevant for me:
* Duration (Lifetime of one cycle): float
* Looping: boolean
* Prewarm: boolean (only applicable if Looping = true)
* (Start) Lifetime (Lifetime of one particle, equals Duration): float 
* Start Speed: float
* Start rotation: float
* Start Size: (float, float)
* Start color
* Simulation Space: World / Local
* Max Particles: int
* Emission Rate (Num. particles per second): int
* Burst (number of particles to emit at t = 0): integer
* Emission Shape (Sphere / Circle / Cone Surface / Cone Volume): emission shape
* Dampening Threshold Speed (speed at which dampening is applied): float
* Dampening coefficient: float
* Color over lifetime: gradient
* Size of lifetime: tween / spline
* Angular velocity: float
* Inherit velocity (inertia relative to the parent transform in percent): float
* Sub-emitter on Death: function
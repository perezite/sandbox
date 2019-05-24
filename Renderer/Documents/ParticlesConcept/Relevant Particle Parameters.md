The following particle parameters are currently relevant for me:
* Duration (Lifetime of one cycle): float
* Looping: boolean
* Prewarm: boolean (only applicable if Looping = true)
* (Start) Lifetime (Lifetime of one particle, equals Duration): float (OK)
* Start Speed: (float, float) (OK)
* Start rotation: (float, float)
* Start Size: (float, float)
* Start color
* Simulation Space: World / Local
* Max Particles: int (OK)
* Emission Rate (Num. particles per second): int (OK)
* Burst (number of particles to emit at t = 0): integer
* Emission Shape (Sphere / Circle / Cone Surface / Cone Volume): emission shape
* Dampening Threshold Speed (speed at which dampening is applied): float
* Dampening coefficient: float
* Color over lifetime: gradient
* Size over lifetime: tween / spline
* Angular velocity: float
* Inherit velocity (inertia relative to the parent transform in percent): float
* Sub-emitter on Death: function
The following particle parameters are currently relevant for me:
* Duration (Lifetime of one cycle): float (OK)
* Looping: boolean (OK)
* Prewarm: boolean (only applicable if Looping = true) (OK)
* (Start) Lifetime (Lifetime of one particle, equals Duration): float (OK)
* Start Speed: (float, float) (OK)
* Start rotation: (float, float) (OK)
* Start Size: (float, float) (OK)
* Start color: Color (OK)
* Max Particles: int (OK)
* Emission Rate (Num. particles per second): int (OK)
* Burst (number of particles to emit at t = 0): integer (OK)
* Emission Shape: emission shape (OK)
* Dampening Threshold Speed (speed at which dampening is applied): float
* Dampening coefficient: float
* Color over lifetime: gradient
* Size over lifetime: tween / spline
* Angular velocity: float (OK)
* Inherit velocity (inertia relative to the parent transform in percent): float
* Sub-emitter on Death: function
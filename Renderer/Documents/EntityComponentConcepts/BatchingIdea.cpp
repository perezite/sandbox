// scene.createChild<StonyQuad>().useBatch(useBatchInChildren: true)
	// .createChild<FurryBackgroundQuad>().drawBeforeParent()	// draw order (-1, 0) 
	// .createChild<FurryForegroundQuad>()						// draw order (0, 1)
	// .createChild<AnotherForegroundQuad>()					// draw order (0, 2)
	// .createChild<StonyBackgroundQuad>().drawBeforeParent();	// draw order (-1, 3)
	// .createChild<YetAnotherBackgroundQuad>().withDrawOrder(-2);	// draw order (-2, 4)

scene
	.createChild<StonyQuad>().useBatching(useBatchInChildren: true)
	.andCreateChild<FurryBackgroundQuad>().drawBeforeParent()	// draw order (-1, 0) 
	.andCreateChild<FurryForegroundQuad>()						// draw order (0, 1)
	.andCreateChild<AnotherForegroundQuad>()					// draw order (0, 2)
	.andCreateChild<StonyBackgroundQuad>().drawBeforeParent()	// draw order (-1, 3)
	.andCreateChild<YetAnotherBackgroundQuad>().withDrawOrder(-2); // draw order (-2, 4)
	
// order in vector
StonyQuad (drawOrder 0)
FurryBackgroundQuad (drawOrder - 1)
FurryForegroundQuad (drawOrder 0)
AnotherForegroundQuad (drawOrder 0)
StonyBackgroundQuad (drawOrder -1)
YetAnotherBackgroundQuad (drawOrder -2)

// final draw order
// using stable_sort(vector, drawOrder)
YetAnotherBackgroundQuad (drawOrder -2)
FurryBackgroundQuad (drawOrder - 1)
StonyBackgroundQuad (drawOrder -1)
StonyQuad (drawOrder 0)
FurryForegroundQuad (drawOrder 0)
AnotherForegroundQuad (drawOrder 0)
 

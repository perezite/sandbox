https://improve.dk/xmldocument-fluent-interface/

- Scene
	- Entity 1
		- Behavior 1
		- Behavior 2
		- Entity 2
			- Behavior 3
	- Entity 3

// option 
scene.create()
	.withChild<Entity1>()
		.withBehavior<Behavior1>().withSomeField(42)
		.withBehavior<Behavior2>().withSomeOtherField(43)
		.withChild<Entity2>().withPosition(1, 1).withRotation(30)
			.withBehavior<Behavior3>
			.getParent()
		.getParent()
	.withSibling<Entity3>();
	
// option
scene
	.createChild<Entity1>()
		.createBehavior<Behavior1>().withBehavior1Field(42)
		.andCreateBehavior<Behavior2>().withBehavior2Field(43)
		.createChild<Entity2>().withPosition(1, 1).withRotation(30)
			.createBehavior<Behavior3>
			.getParent()
		.getParent()
	.andCreateChild<Entity3>();

// option
scene
	.createChild<Entity1>()
		.createBehavior<Behavior1>().withBehavior1Field(42)
		.createSiblingBehavior<Behavior2>().withBehavior2Field(43)
		.createChild<Entity2>().withPosition(1, 1).withRotation(30)
			.createBehavior<Behavior3>()
			.getParent()
		.getParent()
	.createSibling<Entity3>()

// option
scene.create()
	.withChild<Entity1>()
		.withBehavior<Behavior1>().withBehavior1Field(42)
		.withSibling<Behavior2>().withBehavior2Field(43)
		.withChild<Entity2>().withPosition(1, 1).withRotation(30)
			.withBehavior<Behavior3>()
			.getParent()
		.getParent()
	.withSibling<Entity3>()

// option
scene.create()
	.withChild<Entity1>()
		.withBehavior<Behavior1>().withBehavior1Field(42)
		.andBehavior<Behavior2>().withBehavior2Field(43)
		.withChild<Entity2>().withPosition(1, 1).withRotation(30)
			.withBehavior<Behavior3>()
			.getParent()
		.getParent()
	.andChild<Entity3>()

// option
scene.create()
	.withChild<Entity1>()
		.withBehavior<Behavior1>().withBehavior1Field(42)
		.withBehavior<Behavior2>().withBehavior2Field(43)
		.withChild<Entity2>().withPosition(1, 1).withRotation(30)
			.withBehavior<Behavior3>()
			.getParent()
		.getParent()
	.andChild<Entity3>()

// option 
scene
	.createChild<Entity1>()
		.createBehavior<Behavior1>().withBehavior1Field(42)
		.createAnotherBehavior<Behavior2>().withBehavior2Field(43)
		.createChild<Entity2>().withPosition(1, 1).withRotation(30)
			.createBehavior<Behavior3>
			.getParent()
		.getParent()
	.createAnotherChild<Entity3>();

// option
scene.within()
	.createChild<Entity1>().within()
		.createBehavior<Behavior1>()
		.createBehavior<Behavior2>()
		.createChild<Entity2>().within()
			.createBehavior<Behavior3>
			.endWithin()
		.endWithin()
	.createChild<Entity3>();
	
// option
builder(scene).within()
	.buildChild<Entity1>().within()
		.buildBehavior<Behavior1>()
		.buildBehavior<Behavior2>()
		.buildChild<Entity2>().within()
			.buildBehavior<Behavior3>
			.endWithin()
		.endWithin()
	.buildChild<Entity3>();
	
// option
create(scene)
	.withChild<Entity1>().setPosition(1,2)
		.toChild()
		.withBehavior<Behavior1>()
		.withBehavior<Behavior2>()
		.withChild<Entity2>().toChild()
			.withBehavior<Behavior3>()
			.toParent()
		.toParent()
	.withChild<Entity3>();
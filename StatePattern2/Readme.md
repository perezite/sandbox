I try to implement a state pattern which captures the following coroutine archetypes in C++

private IEnumerator DoStuffAndIdle()
{
	<do stuff>
	while (!someCondition)
	{
		<do stuff>

		yield return null;
	}
	
	<do stuff>
}

private IEnumerator BlockAndDoSutff()
{
	<do stuff>
	yield return AnotherCoroutine();	// block
	<do stuff>
}

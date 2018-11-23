using System;
using System.Collections.Generic;

public static class Program
{
	public static void Main()
	{
		Demo1();
		Console.ReadLine();
	}

	public static void Demo1()
	{
		Game game = new Game();
		game.Run();
	}
}

public class Game
{
	// game actors
	private List<Actor> actors = new List<Actor>();

	// current 'frame'
	private int currentFrame = 0;

	public Game()
	{
		actors.Add(new Hero());
	}

	public void Run()
	{
		while (true)
		{
			Console.Write("Your input (enter for nothing):");
			ConsoleKey key = Console.ReadKey().Key;
			Console.WriteLine();

			Console.WriteLine("Begin Game.Update() and Game.HandleInput()");
			HandleInput(key);
			Update();
			Console.WriteLine("End Game.Update() and Game.HandleInput()");
			Console.WriteLine();
		}
	}

	public void Update()
	{
		foreach (var actor in actors)
		{
			actor.Update();
		}
	}

	public void HandleInput(ConsoleKey key)
	{
		foreach (var actor in actors)
		{
			actor.HandleInput(key);
		}
	}
}

// A game actor
public abstract class Actor
{
	// update the actor
	public abstract void Update();

	// handle input
	public abstract void HandleInput(ConsoleKey key);
}

// A game actor state
public abstract class ActorState
{
	// update
	public abstract ActorState Update();

	// handle input
	public abstract void HandleInput(ConsoleKey key);
}

// A hero in the game
public class Hero : Actor
{
	private ActorState state;

	public Hero()
	{
		state = new HeroStandingState();
	}

	public override void Update()
	{
		// update and handle input
		state = state.Update();

		// render
		Console.WriteLine("Hero.Update() (State = {0})", state);
	}

	public override void HandleInput(ConsoleKey key)
	{
		state.HandleInput(key);
	}
}

// standing state
public class HeroStandingState : ActorState
{
	private bool isJumping = false;

	public override void HandleInput(ConsoleKey key)
	{
		if (key == ConsoleKey.UpArrow)
		{
			isJumping = true;
		}
	}

	public override ActorState Update()
	{
		if (isJumping)
			return new HeroJumpingState();
		return this;
	}
}

// hero jumping state
public class HeroJumpingState : ActorState
{
	private int jumpHeight = 3;

	public override void HandleInput(ConsoleKey key) { }

	public override ActorState Update()
	{
		ActorState newState = this;

		jumpHeight--;
		if (jumpHeight <= 0)
		{
			jumpHeight = 0;
			newState = new HeroStandingState();
		}

		Console.WriteLine("jumpHeight: {0}", jumpHeight);
		
		return newState;
	}
}



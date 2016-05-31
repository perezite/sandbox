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

// A hero in the game
public class Hero : Actor
{
	private enum State { Standing, Jumping };

	private State state;

	private int jumpHeight = 0;

	public override void Update()
	{
		// update
		switch (state)
		{
			case State.Standing:
				UpdateStanding();
				break;
			case State.Jumping:
				UpdateJumping();
				break;
		}
			
		// render
		Console.WriteLine("Hero.Update() (State = {0}, Jump Height = {1})", state, jumpHeight);
	}

	public void UpdateStanding()
	{
	}

	public void UpdateJumping()
	{
		jumpHeight--;
		if (jumpHeight <= 0)
		{
			jumpHeight = 0;
			state = State.Standing;
		}
	}

	public override void HandleInput(ConsoleKey key)
	{
		if (key == ConsoleKey.UpArrow)
		{
			jumpHeight = 3;
			state = State.Jumping;
		}
	}
}


/* #include "coroutine.h"
#include <iostream>

int main()
{
	coroutine test{ [](coroutine & self)
	{
		for (int i = 0; i < 5; ++i)
		{
			std::cout << "coroutine " << i << std::endl;
			self.yield();
		}
	} };
	while (test)
	{
		std::cout << "main" << std::endl;
		test();
	}
	return 0;
}*/


#include <windows.h>
#include <utility>
#include <functional>
#include <memory>
#include <iostream>

class FiberCoroutine
{
public:
	using Yield = std::function<void()>;
	using Run = std::function<void(Yield)>;

	FiberCoroutine()
		: mCurrent(nullptr), mRunning(false)
	{
	}

	~FiberCoroutine()
	{
		if (mCurrent)
			DeleteFiber(mCurrent);
	}

	void setup(Run f)
	{
		if (!mMain)
		{
			mMain = ConvertThreadToFiber(NULL);
		}
		mRunning = true;
		mFunction = std::move(f);

		if (!mCurrent)
		{
			mCurrent = CreateFiber(0,
				&FiberCoroutine::proc, this);
		}
	}

	bool step()
	{
		SwitchToFiber(mCurrent);
		return mRunning;
	}

	void yield()
	{
		SwitchToFiber(mMain);
	}

private:
	void run()
	{
		while (true)
		{
			mFunction([this]
			{ yield(); });
			mRunning = false;
			yield();
		}
	}

	static VOID WINAPI proc(LPVOID data)
	{
		reinterpret_cast<FiberCoroutine*>(data)->run();
	}

	static LPVOID mMain;
	LPVOID mCurrent;
	bool mRunning;
	Run mFunction;
};

LPVOID FiberCoroutine::mMain = nullptr;

int main()
{
	auto coroutine = std::make_shared<FiberCoroutine>();
	coroutine->setup([](FiberCoroutine::Yield yield)
	{
		for (int i = 0; i < 3; ++i)
		{
			std::cout << "Coroutine "
				<< i << std::endl;
			yield();
		}
	});

	int stepCount = 0;
	while (coroutine->step())
	{
		std::cout << "Main "
			<< stepCount++ << std::endl;
	}

	std::cin.get();
}
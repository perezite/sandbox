#include "ucontext.h"
#include <cstdint>
#include <memory>
#include <functional>

struct coroutine
{
	coroutine(const std::function<void(coroutine &)> func, size_t stack_size = 64*1024)
		: stack{ new unsigned char[stack_size] }, func{ func }
	{
		getcontext(&callee);
		callee.uc_link = &caller;
		callee.uc_stack.ss_size = stack_size;
		callee.uc_stack.ss_sp = stack.get();
		makecontext(&callee, reinterpret_cast<void(*)()>(&coroutine_call), 2, reinterpret_cast<size_t>(this) >> 32, this);
	}
	coroutine(const coroutine &) = delete;
	coroutine & operator=(const coroutine &) = delete;
	coroutine(coroutine &&) = default;
	coroutine & operator=(coroutine &&) = default;

	void operator()()
	{
		if (returned) return;
		swapcontext(&caller, &callee);
	}

	operator bool() const
	{
		return !returned;
	}

	void yield()
	{
		swapcontext(&callee, &caller);
	}

private:
	ucontext_t caller;
	ucontext_t callee;
	std::unique_ptr<unsigned char[]> stack;
	std::function<void(coroutine &)> func;
	bool returned = false;

	static void coroutine_call(uint32_t this_pointer_left_half, uint32_t this_pointer_right_half)
	{
		coroutine & this_ = *reinterpret_cast<coroutine *>((static_cast<size_t>(this_pointer_left_half) << 32) + this_pointer_right_half);
		this_.func(this_);
		this_.returned = true;
	}
};
#include <coroutine>
#include <iostream>

struct generator
{
    struct promise_type
    {
        int current_value;

        generator get_return_object()
        {
            return generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept
        {
            return {};
        }

        std::suspend_always final_suspend() noexcept
        {
            return {};
        }

        void return_void()
        {
        }

        std::suspend_always yield_value(int value) noexcept
        {
            current_value = value;
            return {};
        }

        void unhandled_exception()
        {
            std::terminate();
        }
    };

    std::coroutine_handle<promise_type> coroutine;

    explicit generator(std::coroutine_handle<promise_type> coroutine) : coroutine(coroutine)
    {
    }

    ~generator()
    {
        if (coroutine)
        {
            coroutine.destroy();
        }
    }

    int get_current_value() const
    {
        return coroutine.promise().current_value;
    }

    void resume()
    {
        coroutine.resume();
    }

    bool done() const
    {
        return coroutine.done();
    }
};

generator numbers()
{
    co_yield 1;
    co_yield 2;
    co_yield 3;
}

int main()
{
    generator gen = numbers();
    while (!gen.done())
    {
        std::cout << gen.get_current_value() << std::endl;
        gen.resume();
    }
    return 0;
}

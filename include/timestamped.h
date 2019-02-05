#ifndef TIMESTAMPED_H
#define TIMESTAMPED_H

#include <chrono>

template <typename T> class timestamped
{
    T data;
    using clock = std::chrono::steady_clock;
    clock::time_point last_updated;

    public:

    timestamped() { };

    template <typename B> timestamped(const B &init)
    {
        update(init);
    }

    operator T () const
    {
        return data;
    }

    const T& operator () () const
    {
        return data;
    }

    timestamped<T>& operator = (const T& newdata)
    {
        update(newdata);
        return *this;
    }

    const T& get() const
    {
        return data;
    }

    void update()
    {
        last_updated = clock::now();
    }

    void update(T newdata)
    {
        data = newdata;
        update();
    }

    std::chrono::microseconds age() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>
            (clock::now() - last_updated);
    }
};

#endif // TIMESTAMPED_H


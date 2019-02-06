#ifndef TIMESTAMPED_H
#define TIMESTAMPED_H

#include <chrono>

/// \brief Wrapper class which adds a timestamp.
/// \details A templated class which provides the timestamp
/// for the last write access to the contained data.
/// Useful for automatically tracking the age of a
/// sensor measurement.
/// \param T The contained type.
template <typename T> class timestamped
{
    /// The contained data.
    T data;
    /// A convenience typedef.
    using clock = std::chrono::steady_clock;
    /// The timestamp of the last update to data.
    clock::time_point last_updated;

    public:

    /// \brief Default constructor.
    /// \details Default-initializes the data
    /// and sets the timestamp to the clock's epoch.
    timestamped() { };

    /// \brief Constructor which takes a B data.
    /// \details Will try to construct a new T using
    /// the provided data, and will update the
    /// timestamp.
    /// \param init The data to initialize with.
    template <typename B> timestamped(const B &init)
    {
        update(init);
    }

    /// \brief Provides const access to the underlying data.
    /// \return Read only access to the contained T.
    operator T () const
    {
        return data;
    }

    /// \brief Provides const access to the underlying data.
    /// \return Read only access to contained data.
    const T& operator () () const
    {
        return data;
    }

    /// \brief Assign to the contained data.
    /// \details Updates the data field as well as the timestamp.
    /// \param newdata The data to be stored.
    /// \return A reference to this object.
    timestamped<T>& operator = (const T& newdata)
    {
        update(newdata);
        return *this;
    }

    /// \brief Provides const reference to the underlying data.
    /// \return A const-ref to data.
    const T& get() const
    {
        return data;
    }

    /// \brief Sets the write timestamp to the current time.
    void update()
    {
        last_updated = clock::now();
    }

    /// \brief Updates the data field and the timestamp.
    /// \param newdata The data to be stored.
    void update(T newdata)
    {
        data = newdata;
        update();
    }

    /// \brief Gets the age of the current measurement.
    /// \return The age of the measurement, in chrono::microseconds.
    std::chrono::microseconds age() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>
            (clock::now() - last_updated);
    }
};

#endif // TIMESTAMPED_H


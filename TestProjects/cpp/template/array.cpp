template<typename T, int SIZE> T& array<T, SIZE>::operator [](int i)
    {
    return data_[i];
    }

template<typename T, int SIZE> const T& array<T, SIZE>::get_elem(int i) const
    {
    return data_[i];
    }

template<typename T, int SIZE> void array<T, SIZE>::set_elem(int i, const T& value)
    {
    data_[i] = value;
    }
template<typename T, int SIZE> array<T, SIZE>::operator T*()
    {
    return data_;
    }
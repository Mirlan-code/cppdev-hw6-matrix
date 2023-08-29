#include "lib.h"

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

template <typename T>
auto explode(T&& t, char)
{
    return std::forward_as_tuple(std::forward<T>(t));
}

template <typename T, std::size_t I = std::tuple_size<std::decay_t<T>>{}>
auto explode(T&& t, int);

template <typename T, std::size_t... Is>
auto explode(T&& t, std::index_sequence<Is...>)
{
    return std::tuple_cat(explode(std::get<Is>(std::forward<T>(t)), 0)...);
}

template <typename T, std::size_t I>
auto explode(T&& t, int)
{
    return explode(std::forward<T>(t), std::make_index_sequence<I>{});
}

template <typename T, std::size_t... Is>
auto decay_tuple(T&& t, std::index_sequence<Is...>)
{
    return std::make_tuple(std::get<Is>(std::forward<T>(t))...);
}

template <typename T>
auto decay_tuple(T&& t)
{
    return decay_tuple(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::decay_t<T>>{}>{});
}

template <typename T, std::size_t... Is>
auto merge_tuple(T&& t, std::index_sequence<Is...>)
{
    return decay_tuple(std::tuple_cat(explode(std::get<Is>(std::forward<T>(t)), 0)...));
}

template <typename T>
auto merge_tuple(T&& t)
{
    return merge_tuple(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::decay_t<T>>{}>{});
}

template<typename T, T defaultValue, size_t nSize = 2>
class Matrix {
public:
    Matrix() = default;

    size_t size() {
        size_t sz = 0;
        auto iter = Data_.begin();
        while (iter != Data_.end()) {
            size_t curSz = iter->second.size();
            if (curSz == 0) {
                iter = Data_.erase(iter);
            } else {
                ++iter;
            }
            sz += curSz;
        }
        return sz;
    }

    Matrix<T, defaultValue, nSize - 1>& operator[](size_t i) {
        return Data_[i];
    }

    class RecursiveIterator {
        using GlobalIterator = typename unordered_map<size_t, Matrix<T, defaultValue, nSize - 1>>::iterator;
        using CurrentIterator = typename Matrix<T, defaultValue, nSize - 1>::RecursiveIterator;

    public:
        RecursiveIterator(GlobalIterator globalIter, CurrentIterator currentIter)
            : GlobalIter_(globalIter)
            , CurrentIter_(currentIter) {}

        RecursiveIterator operator++() {
            ++CurrentIter_;
            if (GlobalIterUpdated_) {
                CurrentIter_ = GlobalIter_->second.begin();
                GlobalIterUpdated_ = false;
            }

            if (CurrentIter_ != GlobalIter_->second.end()) {

            } else {
                GlobalIter_++;
                GlobalIterUpdated_ = true;
            }
            return *this;
        }

        bool operator!=(const RecursiveIterator& other) {
            return GlobalIter_ != other.GlobalIter_;
        }

        auto operator*() {
            if (GlobalIterUpdated_) {
                CurrentIter_ = GlobalIter_->second.begin();
                GlobalIterUpdated_ = false;
            }

            auto x = make_tuple(GlobalIter_->first, *CurrentIter_);
            return merge_tuple(x);
        }

    private:
        GlobalIterator GlobalIter_;
        bool GlobalIterUpdated_ = false;
        CurrentIterator CurrentIter_;
    };

    RecursiveIterator begin() {
        return RecursiveIterator(Data_.begin(), Data_.begin()->second.begin());
    }

    RecursiveIterator end() {
        return RecursiveIterator(Data_.end(), Data_.begin()->second.begin());
    }

private:
    unordered_map<size_t, Matrix<T, defaultValue, nSize - 1>> Data_;
};

template<typename T, T defaultValue>
class Matrix <T, defaultValue, 1> {
public:
    Matrix() = default;

    size_t size() {
        auto iter = Data_.begin();
        while (iter != Data_.end()) {
            if (iter->second == defaultValue) {
                iter = Data_.erase(iter);
            } else {
                ++iter;
            }
        }
        return Data_.size();
    }

    T& operator[](size_t i) {
        if (!Data_.contains(i)) {
            Data_[i] = defaultValue;
        }
        return Data_[i];
    }

    class RecursiveIterator {
        using GlobalIterator = typename unordered_map<size_t, T>::iterator;
        using CurrentIterator = typename Matrix<T, defaultValue, 1>::RecursiveIterator;
    public:
        RecursiveIterator(GlobalIterator globalIter)
            : GlobalIter_(globalIter) {}

        RecursiveIterator operator++() {
            GlobalIter_++;
            return *this;
        }

        bool operator!=(const RecursiveIterator& other) {
            return GlobalIter_ != other.GlobalIter_;
        }

        tuple<size_t, T> operator*() {
            return {GlobalIter_->first, GlobalIter_->second};
        }

    private:
        GlobalIterator GlobalIter_;
    };

    RecursiveIterator begin() {
        return RecursiveIterator(Data_.begin());
    }

    RecursiveIterator end() {
        return RecursiveIterator(Data_.end());
    }

private:
    unordered_map<size_t, T> Data_;
};

int main (int, char **) {
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0); // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    // выведется одна строка
    // 100100314
    for(auto c : matrix) {
        size_t x;
        size_t y;
        int v;
        tie(x, y, v) = c;
        cout << x << y << v << endl;
    }

    return 0;
}

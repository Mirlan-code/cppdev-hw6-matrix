#pragma once

#include "tuple_helper.h"

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

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
        using GlobalIterator = typename std::unordered_map<size_t, Matrix<T, defaultValue, nSize - 1>>::iterator;
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
    std::unordered_map<size_t, Matrix<T, defaultValue, nSize - 1>> Data_;
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
        using GlobalIterator = typename std::unordered_map<size_t, T>::iterator;
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

        std::tuple<size_t, T> operator*() {
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
    std::unordered_map<size_t, T> Data_;
};
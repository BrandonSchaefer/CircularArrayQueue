//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* * Copyright (C) 2015 Brandon Schaefer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 3 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Authored by: Brandon Schaefer <brandontschaefer@gmail.com>
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <algorithm>
#include <memory>

namespace ds
{

namespace
{
    size_t const default_max_size = 2;
}

template <typename T, typename Alloc = std::allocator<T>>
class queue
{
public:
    queue() = default;

    explicit queue(size_t size)
        : queue_(allocator_.allocate(size)),
          max_size_(size)
    {
    }

    ~queue()
    {
        allocator_.deallocate(queue_, max_size_);
    }

    queue(queue const& q)
        : queue_(allocator_.allocate(q.max_size_)),
          front_(q.front_),
          back_(q.back_),
          max_size_(q.max_size_)
    {
        std::uninitialized_copy(q.queue_, q.queue_ + q.max_size_, queue_);
    }

    // init_list ctor
    queue(std::initializer_list<T> const& s)
        : queue_(allocator_.allocate(size + 1)),
          back_(s.size()),
          max_size_(s.size() + 1)
    {
        std::uninitialized_copy(s.begin(), s.end(), queue_);
    }

    // move ctor
    queue(queue&& q)
        : queue_(q.queue_),
          front_(q.front_),
          back_(q.back_),
          max_size_(q.max_size_)
    {
        q.queue_ = nullptr;
        q.front_ = 0;
        q.back_  = 0;
        q.max_size_ = 0;
    }

    // copy assignment
    queue& operator=(queue const& q)
    {
        queue tmp{q};
        std::swap(tmp, *this);
        return *this;
    }

    // move assignment
    queue& operator=(queue&& q)
    {
        std::swap(queue_, q.queue_);
        std::swap(front_, q.front_);
        std::swap(back_,  q.back_);
        std::swap(max_size_, q.max_size_);
        return *this;
    }

    void resize(size_t new_size)
    {
        auto new_queue = allocator_.allocate(new_size + 1);
        auto elem_size = std::min(new_size, size());

        if (elem_size > 0)
        {
            // We are linear copy front --> elem_size
            if (front_ < back_)
            {
                std::uninitialized_copy(queue_ + front_, queue_ + elem_size, new_queue);
            }
            // We are split up, need to copy front --> end_of_elems, then 0 --> back_
            else
            {
                size_t front_half = max_size_ - front_;
                size_t back_half  = back_ - 1;

                 // We are shinking, and only need to copy up to elem_size (not past)
                 if (elem_size < back_half)
                     back_half = elem_size;

                 std::uninitialized_copy_n(queue_ + front_, front_half, new_queue);
                 std::uninitialized_copy_n(queue_, back_half, new_queue + front_half);
            }
        }

        allocator_.deallocate(queue_, max_size_);

        back_     = elem_size;
        front_    = 0;
        max_size_ = new_size + 1;
        queue_    = new_queue;
    }

    void enqueue(T const& value)
    {
        if (size() + 1 >= max_size_)
            resize(max_size_ * 2);

        queue_[back_] = value;
        back_ = (back_ + 1) % max_size_;
    }

    T dequeue()
    {
        if (size() <= 0)
            throw std::out_of_range("Queue is empty, no elements left.");

        auto elem = queue_[front_];
        queue_[front_] = T{};
        front_ = (front_ + 1) % max_size_;

        return elem;
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_t size() const
    {
        if (max_size_ > 0)
            return (max_size_ - (front_ -  back_)) % max_size_;

        return 0;
    }

private:
    std::allocator<T> allocator_;
    T* queue_{allocator_.allocate(default_max_size)};

    size_t front_{0};
    size_t back_{0};
    size_t max_size_{default_max_size};
};

} // namespace ds

#endif // QUEUE_H

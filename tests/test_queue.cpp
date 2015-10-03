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

#include "queue.h"

#include <gtest.h>

class TestQueue : public ::testing::Test
{
public:
    TestQueue()
    {
    }

    ds::queue<int> q;
};

TEST_F(TestQueue, DefaultConstructor)
{
    EXPECT_TRUE(q.empty());
}

TEST_F(TestQueue, CopyConstructor)
{
    int elem = 10;
    q.enqueue(elem);
    ds::queue<int> q2(q);

    EXPECT_EQ(q2.dequeue(), elem);
    EXPECT_TRUE(q2.empty());
}

TEST_F(TestQueue, MoveConstructor)
{
    ds::queue<int> q2(std::move(ds::queue<int>()));
    EXPECT_TRUE(q2.empty());
}

TEST_F(TestQueue, MoveAssignmentConstructor)
{
    q = ds::queue<int>();
    EXPECT_TRUE(q.empty());
}

TEST_F(TestQueue, CopyAssignmentConstructor)
{
    int elem = 10;
    q.enqueue(elem);

    ds::queue<int> q2 = q;

    EXPECT_EQ(q2.dequeue(), elem);
    EXPECT_TRUE(q2.empty());
}

TEST_F(TestQueue, EnqueueDequeue)
{
    int elem = 10;
    q.enqueue(elem);
    EXPECT_EQ(q.dequeue(), elem);
    EXPECT_TRUE(q.empty());
}

TEST_F(TestQueue, IteratorBegin)
{
    int elem = 1;
    q.enqueue(elem);

    EXPECT_EQ(*q.begin(), elem);
}

TEST_F(TestQueue, IteratorWritable)
{
    int elem = 1;
    q.enqueue(elem);
    q.begin() = elem + 1;

    EXPECT_EQ(*q.begin(), elem + 1);
}

TEST_F(TestQueue, IteratorAssignable)
{
    int elem = 1;
    q.enqueue(elem);
    q.enqueue(elem + 1);
    auto it = q.begin();
    it = ++q.begin();

    EXPECT_EQ(*it, elem + 1);
}

TEST_F(TestQueue, IteratorRange)
{
    size_t elems = 10;
    for (size_t i = 0; i < elems; i++)
        q.enqueue(i);

    int i = 0;
    for (auto it = q.begin(); it != q.end(); ++it)
        EXPECT_EQ(*it, i++);

    EXPECT_EQ(q.size(), elems);
}

TEST_F(TestQueue, IteratorFind)
{
    int elems = 10;
    for (int i = 0; i < elems; i++)
        q.enqueue(i);

    auto it = q.find(elems / 2);
    EXPECT_EQ(*it, elems / 2);
}

TEST_F(TestQueue, IteratorNotFind)
{
    int elems = 10;
    for (int i = 0; i < elems; i++)
        q.enqueue(i);

    auto it = q.find(elems + 1);
    EXPECT_EQ(it, q.end());
}

TEST_F(TestQueue, DequeueEmpty)
{
    try
    {
        q.dequeue();
        FAIL() << "Expected std::out_of_range";
    }
    catch (std::out_of_range const& err)
    {
        // yay we pass
    }
    catch (...)
    {
        FAIL() << "Expected std::out_of_range";
    }
}

TEST_F(TestQueue, ResizeShrinkWithElements)
{
    size_t elems = 10;
    for (size_t i = 0; i < elems; i++)
        q.enqueue(i);

    EXPECT_EQ(q.size(), elems);

    q.resize(elems / 2);
    EXPECT_EQ(q.size(), elems / 2);
}

TEST_F(TestQueue, EnqueueDequeueLargeNumber)
{
    size_t elems = 10000;
    for (size_t i = 0; i < elems; i++)
        q.enqueue(i);

    for (size_t i = 0; i < elems / 2; i++)
        EXPECT_EQ(q.dequeue(), (int)i);

    for (size_t i = 0; i < elems / 2; i++)
        q.enqueue(i);

    EXPECT_EQ(q.size(), elems);
}

TEST_F(TestQueue, ResizeEmptyQueue)
{
    EXPECT_TRUE(q.empty());
    q.resize(5);
}

TEST_F(TestQueue, ShrinkEnqueueDequeueLargeNumber)
{
    size_t elems = 10000;
    for (size_t i = 0; i < elems; i++)
        q.enqueue(i);

    q.resize(elems / 2);
    EXPECT_EQ(q.size(), elems / 2);

    for (size_t i = 0; i < elems / 4; i++)
        EXPECT_EQ(q.dequeue(), (int)i);

    for (size_t i = 0; i < elems / 4; i++)
        q.enqueue(i);

    EXPECT_EQ(q.size(), elems / 2);
}

TEST_F(TestQueue, EnqueueDequeueWrapResize)
{
    ds::queue<int> set_size(2);

    // Move b up 1, then front up 1, then back twice to wrap and resize
    set_size.enqueue(1);
    set_size.dequeue();
    set_size.enqueue(2);
    set_size.enqueue(3);

    EXPECT_EQ(set_size.size(),    (size_t)2);
    EXPECT_EQ(set_size.dequeue(), 2);
    EXPECT_EQ(set_size.dequeue(), 3);
    EXPECT_EQ(set_size.size(),    (size_t)0);
}

TEST_F(TestQueue, EnqueueDequeueFIFO)
{
    size_t elems = 1000;
    for (size_t i = 0; i < elems; i++)
        q.enqueue(i);

    for (size_t i = 0; i < elems; i++)
        EXPECT_EQ(q.dequeue(), (int)i);

    EXPECT_TRUE(q.empty());
}

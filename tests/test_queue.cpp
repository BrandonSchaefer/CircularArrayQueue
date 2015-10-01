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
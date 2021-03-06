/*
 * Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     zhangsheng <zhangsheng@uniontech.com>
 *
 * Maintainer: zhangsheng <zhangsheng@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "vfs/dfmvfsmanager.h"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>


namespace  {

class TestDFMVfsManager: public testing::Test {
public:
    DFM_NAMESPACE::DFMVfsManager *m_manager {nullptr};
    void SetUp() override
    {
        m_manager = new DFM_NAMESPACE::DFMVfsManager;
    }

    void TearDown() override
    {
        delete m_manager;
    }
};
}

TEST_F(TestDFMVfsManager, getVfsList)
{
    EXPECT_EQ(m_manager->getVfsList().size(), 0);
}

TEST_F(TestDFMVfsManager, attach)
{
    EXPECT_FALSE(m_manager->attach(DUrl::fromLocalFile("/")));
}

TEST_F(TestDFMVfsManager, handler)
{
    m_manager->setEventHandler(nullptr);
    EXPECT_EQ(m_manager->eventHandler(), nullptr);
}

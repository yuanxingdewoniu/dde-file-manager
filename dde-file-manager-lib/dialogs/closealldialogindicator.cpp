/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *               2016 ~ 2018 dragondjf
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             zccrs<zhangjide@deepin.com>
 *             Tangtong<tangtong@deepin.com>
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

#include "closealldialogindicator.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QDBusInterface>
#include "shutil/fileutils.h"
#include "accessible/libframenamedefine.h"

CloseAllDialogIndicator::CloseAllDialogIndicator(QWidget *parent) : DAbstractDialog(parent)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::NoFocus);
    setObjectName(DIALOGS_CLOSE_ALL_DIALOG_INDICATOR);

    initUI();
    initConnect();
}

CloseAllDialogIndicator::~CloseAllDialogIndicator()
{

}

void CloseAllDialogIndicator::initUI()
{
    resize(QSize(400, 50));

    m_messageLabel = new QLabel(this);
    m_messageLabel->setObjectName(DIALOGS_CLOSE_ALL_DIALOG_INDICATOR_MSG_LABEL);

    m_closeButton = new QPushButton(tr("Close all"), this);
    m_closeButton->setObjectName(DIALOGS_CLOSE_ALL_DIALOG_INDICATOR_CLOSE_BUTTON);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_messageLabel, Qt::AlignCenter);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(m_closeButton, Qt::AlignRight);
    mainLayout->setContentsMargins(25, 5, 25, 5);
    setLayout(mainLayout);

    setTotalMessage(0, 0);
}

void CloseAllDialogIndicator::initConnect()
{
    connect(m_closeButton, &QPushButton::clicked, this, &CloseAllDialogIndicator::allClosed);
}

void CloseAllDialogIndicator::setTotalMessage(qint64 size, int count)
{
    QString message = tr("Total size: %1, %2 files").arg(FileUtils::formatSize(size), QString::number(count));
    m_messageLabel->setText(message);
}

void CloseAllDialogIndicator::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        return;
    }
    QDialog::keyPressEvent(event);
}

void CloseAllDialogIndicator::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    //! task 36981 使用Dbus获取dock高度计算对话框显示位置
    QDBusInterface deepin_dockInfo("com.deepin.dde.daemon.Dock",
                                   "/com/deepin/dde/daemon/Dock",
                                   "com.deepin.dde.daemon.Dock",
                                   QDBusConnection::sessionBus(), this);

    int dockHeight = 0;
    if(deepin_dockInfo.isValid())
    {
        QVariant temp = deepin_dockInfo.property("WindowSizeEfficient");
        dockHeight = temp.toInt();
    }

    QRect screenGeometry = qApp->desktop()->availableGeometry();

    int geometryHeight = screenGeometry.height() - dockHeight;
    int geometryWidth = screenGeometry.width();

    move((geometryWidth - width()) / 2, geometryHeight - height());

    return DAbstractDialog::showEvent(event);
}


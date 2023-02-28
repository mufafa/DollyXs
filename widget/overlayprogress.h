/*
 * This file is part of the XXX distribution (https://github.com/xxxx or http://xxx.github.io).
 * Copyright (c) 2023 Mustafa Oksuz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * file : overlayprogress.h
 * date : 22.02.2023
 */
#pragma once

#include <QObject>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>

class OverlayProgress : public QWidget
   {
   Q_OBJECT
public:
   explicit OverlayProgress(QWidget* parent = nullptr, const int duration = 370);
   void show(const bool& show = true);

private:
    QGraphicsOpacityEffect fade_effect;
    QPropertyAnimation animation;
    QTimer timer;
    int theRotate{0};

protected:

    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject* obj, QEvent* ev) override;
    bool event(QEvent* ev) override;

private:
    void newParent();
    void drawProgress(QPainter *painter);

   };

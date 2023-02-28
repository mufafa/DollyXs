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
 * file : overlayprogress.cpp
 * date : 22.02.2023
 */
#include "overlayprogress.h"
#include <QPainter>

OverlayProgress::OverlayProgress(QWidget* parent, const int duration)
    : QWidget{ parent }
    , fade_effect(this)
    , animation(&fade_effect, "opacity")
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_NoSystemBackground);
    newParent();
    setGraphicsEffect(&fade_effect);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);
    animation.setEasingCurve(QEasingCurve::InOutQuint);
    animation.setDuration(duration);
    setVisible(false);
    setEnabled(false);

    //selected item focus ring is unnecessary
    parent->setFocus();

    connect(&timer, &QTimer::timeout, this, [this]()
		{
		    theRotate += 1;
		    theRotate %= 360;
		    update();
		});

}

void OverlayProgress::newParent()
{
    if (!parent()) return;
    parent()->installEventFilter(this);
    raise();
   }

void OverlayProgress::drawProgress(QPainter *painter)
   {
      painter->setPen(QPen(Qt::darkGray, 3));
      const int radius = 25; //yarıçap
      const int step = 6*2;     //dönüş sayısı

      for (int i = 0; i < 360; i += step)
      {
	 if (i % 30 == 0) {
	       painter->save();
	       painter->setPen(QPen(Qt::red, 5));
	       painter->restore();
	    }
	 painter->drawLine(0, -radius, 0, -(radius + 12));
	 painter->rotate(-step); //saat yönünün tersi
      }

   }

bool OverlayProgress::eventFilter(QObject* obj, QEvent* ev)
{
    if (obj == parent()) {
	if (ev->type() == QEvent::Resize)
	    resize(static_cast<QResizeEvent*>(ev)->size());
	else if (ev->type() == QEvent::ChildAdded)
	    raise();
    }
    return QWidget::eventFilter(obj, ev);
}

bool OverlayProgress::event(QEvent* ev)
{
    if (ev->type() == QEvent::ParentAboutToChange) {
	if (parent()) parent()->removeEventFilter(this);
    }
    else if (ev->type() == QEvent::ParentChange)
	newParent();
    return QWidget::event(ev);
}

void OverlayProgress::show(const bool& show)
{
    animation.stop();
    animation.setStartValue(animation.currentValue());
    animation.setEndValue(show ? 1.0 : 0.0); // show or hide
    animation.start();
    if (show) {
	setVisible(true);
	setEnabled(true);
    }
    else {
	setEnabled(false);
    }
}

void OverlayProgress::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QColor white(Qt::darkGray);
    white.setAlphaF(0.65);

    painter.fillRect(this->rect(), white);
    QPen pen(Qt::darkGray);
    painter.setPen(pen);
    painter.drawRect(this->rect().adjusted(0, 0, -1, -1));

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //painteri ortaya getirelim
    painter.translate(width() / 2, height() / 2);
    {
       painter.save();
       painter.rotate(-theRotate);
       drawProgress(&painter);
       painter.restore();
    }
   }

void OverlayProgress::showEvent(QShowEvent *event)
   {
      timer.start(10);
      QWidget::showEvent(event);
   }







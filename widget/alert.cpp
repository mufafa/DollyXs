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
 * file : alert.cpp
 * date : 10.02.2023
 */
#include "alert.h"
#include "ui_alert.h"

Alert::Alert(const QString &header, const QString &message, QWidget *parent) :
   QDialog(parent), m_header(header), m_message(message), ui(new Ui::Alert)
   {
      setWindowFlag(Qt::Tool, true);
      ui->setupUi(this);
      ui->headerLabel->setText(header);
      ui->messageLabel->setText(message);
      connect(ui->closeButton, &QPushButton::clicked, this, &Alert::onCloseButton);

      ui->messageLabel->setText(message);
   }

Alert::~Alert()
   {
      delete ui;
   }

void Alert::onCloseButton(bool)
   {
      this->close();
   }

void Alert::showEvent(QShowEvent *event)
   {
      setFixedSize(window()->sizeHint());
      QDialog::showEvent(event);
   }

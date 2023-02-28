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
 * file : alert.h
 * date : 10.02.2023
 */
#pragma once

#include <QDialog>

namespace Ui {
      class Alert;
   }

class Alert : public QDialog
   {
   Q_OBJECT
   const QString& m_header;
   const QString& m_message;
public:
   explicit Alert(const QString& header, const QString& message, QWidget *parent = nullptr);
   ~Alert();

private:
   Ui::Alert *ui;
private slots:
   void onCloseButton(bool value);


   // QWidget interface
protected:
   void showEvent(QShowEvent *event);
   };


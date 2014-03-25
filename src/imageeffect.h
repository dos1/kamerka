/*
 * Copyright (c) Dejan Noveski <dr.mote@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef IMAGEEFFECT_H
#define IMAGEEFFECT_H

#include <QObject>


class ImageEffect: public QObject{
    Q_OBJECT
    Q_ENUMS(Effect)

public:

    enum Effect {
        Effect_None,
        Effect_Grey,
        Effect_Invert,
        Effect_Mono,
        Effect_Smurf,
        Effect_Implode,
        Effect_Explode
    };

    static void invert(QImage &image);
    static void mono(QImage &image);
    static void grey(QImage &image);
    static void smurf(QImage &image);
    static void implode(QImage &image);
    static void explode(QImage &image);
    static void applyEffect(QImage &image, int effect = Effect_None);

};


#endif // IMAGEEFFECT_H

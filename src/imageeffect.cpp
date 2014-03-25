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

#include <QImage>
#include <Qt>
#include <QRgb>
#include "imageeffect.h"
#include <qimageblitz/qimageblitz.h>

void ImageEffect::grey(QImage &i) {

    QRgb *px;
    int g;

    for(int y = 0;y < i.height();y++) {
        px = (QRgb*)i.scanLine(y);

        for(int x = 0;x < i.width();x++) {
            g = (int)(qRed(*px) * 30 + qGreen(*px) * 59 + qRed(*px) * 11)/100;
            *px = qRgb(g, g, g);
            px++;
        }
    }
}

void ImageEffect::invert(QImage &i) {
    i.invertPixels();
}

void ImageEffect::mono(QImage &i) {
    i = i.convertToFormat(QImage::Format_Mono);
}

void ImageEffect::smurf(QImage &i) {
    i = i.rgbSwapped();
}

void ImageEffect::implode(QImage &i) {
    i = Blitz::implode(i, 0.3);

}

void ImageEffect::explode(QImage &i) {
    i = Blitz::implode(i, -0.3);

}

void ImageEffect::applyEffect(QImage &i, int effect) {
    switch(effect)
    {
        case ImageEffect::Effect_None:
            break;
        case ImageEffect::Effect_Grey:
            grey(i); break;
        case ImageEffect::Effect_Invert:
            invert(i); break;
        case ImageEffect::Effect_Mono:
            mono(i); break;
        case ImageEffect::Effect_Smurf:
            smurf(i); break;
        case ImageEffect::Effect_Implode:
            implode(i); break;
        case ImageEffect::Effect_Explode:
            explode(i); break;
    }
}


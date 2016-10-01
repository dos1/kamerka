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

#include <QtGui/QImage>
#include <Qt>
#include <QRgb>
#include "imageeffect.h"
#include <qimageblitz/qimageblitz.h>

void ImageEffect::grey(QImage &i) {
    Blitz::grayscale(i, true);
}

void ImageEffect::invert(QImage &i) {
    Blitz::invert(i);
}

void ImageEffect::equalize(QImage &i) {
    Blitz::equalize(i);
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

void ImageEffect::charcoal(QImage &i) {
    i = Blitz::charcoal(i);
}

void ImageEffect::edge(QImage &i) {
    i = Blitz::edge(i);
}

void ImageEffect::emboss(QImage &i) {
    i = Blitz::emboss(i, 0, 0.8, Blitz::Low);
}

void ImageEffect::swirl(QImage &i) {
    i = Blitz::swirl(i);
}

void ImageEffect::oilPaint(QImage &i) {
    i = Blitz::oilPaint(i, 0, Blitz::Low);
}

void ImageEffect::wave(QImage &i) {
    i = Blitz::wave(i);
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
    case ImageEffect::Effect_Equalize:
        equalize(i); break;
    case ImageEffect::Effect_Smurf:
        smurf(i); break;
    case ImageEffect::Effect_Implode:
        implode(i); break;
    case ImageEffect::Effect_Explode:
        explode(i); break;
    case ImageEffect::Effect_Charcoal:
        charcoal(i); break;
    case ImageEffect::Effect_Edge:
        edge(i); break;
    case ImageEffect::Effect_Emboss:
        emboss(i); break;
    case ImageEffect::Effect_Swirl:
        swirl(i); break;
    case ImageEffect::Effect_OilPaint:
        oilPaint(i); break;
    case ImageEffect::Effect_Wave:
        wave(i); break;
    }
}


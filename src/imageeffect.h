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
#include <QImage>

class ImageEffect: public QObject{
		Q_OBJECT
		Q_ENUMS(Effect)

	public:

		enum Effect {
			Effect_None,
			Effect_Grey,
			Effect_Invert,
			Effect_Equalize,
			Effect_Smurf,
			Effect_Implode,
			Effect_Explode,
			Effect_Charcoal,
			Effect_Edge,
			Effect_Emboss,
			Effect_Swirl,
			Effect_OilPaint,
			Effect_Wave
		};

		static void grey(QImage &image);
		static void invert(QImage &image);
		static void equalize(QImage &image);
		static void smurf(QImage &image);
		static void implode(QImage &image);
		static void explode(QImage &image);
		static void charcoal(QImage &image);
		static void edge(QImage &image);
		static void emboss(QImage &image);
		static void swirl(QImage &image);
		static void oilPaint(QImage &image);
		static void wave(QImage &image);
		static void applyEffect(QImage &image, int effect = Effect_None);

};


#endif // IMAGEEFFECT_H

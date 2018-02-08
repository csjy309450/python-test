/******************************************************************************
 * File:   NodeShape.h
 * Author: Carsten Gutwenger
 *
 * Defines class NodeShape (base class for node shapes) and class
 * NodeShapeFactory, which creates node shape objects.
 *
 ******************************************************************************
 *
 * Copyright (c) 2010-2011 Carsten Gutwenger
 * All rights reserved.
 *
 * This file is part of gml2pic.
 *
 * gml2pic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gml2pic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gml2pic. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/


#ifndef _NODE_SHAPE_H
#define _NODE_SHAPE_H


#include <QPainter>


/******************************************************************************
 *                             ENUM Shape
 *
 * Encodes the possible node shapes.
 *
 * inverted means: flipped vertically
 ******************************************************************************/

enum Shape {
	shRect,               // rectangle
	shRoundedRect,        // rectangle with rounded corners
	shEllipse,            // ellipse
	shTriangle,           // isosceles triangle (base side down)
	shPentagon,           // pentagon
	shHexagon,            // hexagon
	shOctagon,            // octagon
	shRhomb,              // rhomb (=diamond)
	shTrapeze,            // trapeze (upper side shorter)
	shParallelogram,      // parallelogram (slanted to the right)
	shInvTriangle,        // isosceles triangle (base side up)
	shInvTrapeze,         // inverted trapeze  (upper side longer)
	shInvParallelogram    // inverted parallelogram (slanted to the left)
};


/******************************************************************************
 *                               CLASS NodeShape
 *
 * Defines the inteface for node shapes.
 ******************************************************************************/

class NodeShape
{
public:
	NodeShape() { }
	virtual ~NodeShape() { }

	// returns the width of the shape
	virtual qreal width() = 0;

	// returns the height of the shape
	virtual qreal height() = 0;

	// draws the shape with the painter at position (x,y) (upper left corner)
	virtual void draw(QPainter &painter, qreal x, qreal y) = 0;

	// returns the bounding box for a node label
	virtual QRectF bbLabel(qreal x, qreal y, qreal dm) = 0;

	// returns true if p is contained in the region covered by the shape (placed at
	// position (x,y) and drawn with line width lw)
	virtual bool contains(qreal x, qreal y, qreal lw, QPointF p);

	// returns the cut point of the shape (placed at (x,y) and drawn with line width lw)
	// with a straight line from ps (in shape) to pt (outside of shape)
	virtual QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt);

protected:
	static bool rightOf(QPointF p, QPointF q, QPointF z) {
		qreal vx = q.x() - p.x();
		qreal vy = q.y() - p.y();
		return vy*z.x() - vx*z.y() > vy*p.x() - vx*p.y();
	}

	static bool inPolygon(QPointF *p, int n, QPointF q);
	static bool inCircle(qreal mx, qreal my, qreal r, QPointF p) {
		qreal dx = p.x()-mx;
		qreal dy = p.y()-my;
		return dx*dx + dy*dy <= r*r;
	}

	static qreal cutpoint(QPointF ps, QPointF pt, QPointF qs, QPointF qt);
	static QPointF cutpoint(QPointF *p, int n, QPointF ps, QPointF pt);
	static QPointF cutCircle(qreal mx, qreal my, qreal r, QPointF ps, QPointF pt);

};


/******************************************************************************
 *                             CLASS NodetShapeFactory
 *
 * Creates node shapes.
 ******************************************************************************/

class NodeShapeFactory
{
public:
	// creates a node shape with size (w,h)
	static NodeShape *create(Shape shape, qreal w, qreal h);
};


#endif

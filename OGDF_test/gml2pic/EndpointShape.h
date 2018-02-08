/******************************************************************************
 * File:   EndpointShape.h
 * Author: Carsten Gutwenger
 *
 * Defines class EndpointShape (base class for endpoint shapes) and class
 * EndpointShapeFactory, which creates endpoint shape objects.
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


#ifndef _ENDPOINT_SHAPE_H
#define _ENDPOINT_SHAPE_H


#include <qpainter.h>


/******************************************************************************
 *                             ENUM EndpointType
 *
 * Encodes the possible endpoint shapes.
 *
 * open means: different fill and stroke color
 ******************************************************************************/

enum EndpointType {
	stNone,         // no shape
	stArrow,        // arrow head
	stTriangle,     // triangular arrow head
	stOTriangle,    // open triangular arrow head
	stVee,          // vee-like arrows head
	stTee,          // tee-like stop marker
	stCircle,       // circle
	stOCircle,      // open circle
	stBox,          // box (=rectangle)
	stOBox,         // open box (=rectangle)
	stRhomb,        // rhomb (=diamond)
	stORhomb        // open rhomb (=diamond)
};


/******************************************************************************
 *                              CLASS EndpointShape
 *
 * Defines the inteface for endpoint shapes.
 ******************************************************************************/

class EndpointShape
{
public:
	EndpointShape() { }
	virtual ~EndpointShape() { }

	// draws the shape with the painter
	virtual void draw(QPainter &painter) = 0;

	// returns the length of the shape, i.e., the length of the segement that must be
	// removed from the edge polyline
	virtual qreal length() = 0;

	// returns the desired pen width for the shape, given the line width lw of the edge
	virtual qreal penWidth(qreal lw) { return cOutlineFactor*lw; }

	// returns the desired line cap style for the shape
	virtual Qt::PenCapStyle capStyle() { return Qt::FlatCap; }

	// returns true if type specifies an open shape
	static bool isOpen(EndpointType type) {
		switch(type) {
		case stOTriangle:
		case stOCircle:
		case stOBox:
		case stORhomb:
			return true;
		}
		return false;
	}

	static const qreal cMinEdgeWidth;   // minimal width of an edge
	static const qreal cOutlineFactor;  // usually, shape pen width is cOutlineFactor * edge width
};


/******************************************************************************
 *                             CLASS EndpointShapeFactory
 *
 * Creates endpoint shapes.
 ******************************************************************************/

class EndpointShapeFactory
{
public:
	// creates a new endpoint shape for given type, line width, and size
	//   lw   line width of the edge
	//   sz   size of the shape (= scale factor)
	static EndpointShape *create(EndpointType type, qreal lw, qreal sz);
};


#endif

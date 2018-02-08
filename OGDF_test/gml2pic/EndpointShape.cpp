/******************************************************************************
 * File:   EndpointShape.cpp
 * Author: Carsten Gutwenger
 *
 * Defines and implements endpoint shape classes.
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


#include "EndpointShape.h"
#include <ogdf/basic/basic.h>

using namespace ogdf;


//---------------------------------------------------------
// EndpointShape
//---------------------------------------------------------

const qreal EndpointShape::cMinEdgeWidth = 0.01;
const qreal EndpointShape::cOutlineFactor = 0.75;


//---------------------------------------------------------
// EndpointShapeTriangle
//---------------------------------------------------------

class EndpointShapeTriangle : public EndpointShape
{
public:
	EndpointShapeTriangle(qreal lw, qreal sz) {
		sz *= lw;
		lw = penWidth(lw);
		qreal dx = s_headHalfWidth;
		qreal dy = s_headHeight;
		qreal t = 0.5*lw * sqrt(dx*dx+dy*dy) / dx;

		m_path.moveTo(0,t);
		m_path.lineTo(sz*dx, sz*dy+t);
		m_path.lineTo(-sz*dx, sz*dy+t);
		m_path.closeSubpath();

		m_length = sz*dy + t;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.drawPath(m_path);
	}

private:
	QPainterPath m_path;
	qreal m_length;

	static qreal s_headHalfWidth;
	static qreal s_headHeight;
};

qreal EndpointShapeTriangle::s_headHalfWidth  = 3.0;
qreal EndpointShapeTriangle::s_headHeight = 8.0;


//---------------------------------------------------------
// EndpointShapeArrow
//---------------------------------------------------------

class EndpointShapeArrow : public EndpointShape
{
public:
	EndpointShapeArrow(qreal lw, qreal sz) {
		sz *= lw;
		lw = penWidth(lw);
		qreal dx = s_headHalfWidth;
		qreal dy = s_height;
		qreal t = 0.5*lw * sqrt(dx*dx+dy*dy) / dx;

		qreal ih = sz*s_innerHeight;
		dx *= sz; dy *= sz;

		m_path.moveTo(0,t);
		m_path.lineTo(dx, dy+t);
		m_path.lineTo(0, ih+t);
		m_path.lineTo(-dx, dy+t);
		m_path.closeSubpath();

		m_length = ih + t + 0.5*lw;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.drawPath(m_path);
	}

private:
	QPainterPath m_path;
	qreal m_length;

	static qreal s_headHalfWidth;
	static qreal s_height;
	static qreal s_innerHeight;
};

qreal EndpointShapeArrow::s_headHalfWidth  = 3.0;
qreal EndpointShapeArrow::s_height = 8.0;
qreal EndpointShapeArrow::s_innerHeight = 5;


//---------------------------------------------------------
// EndpointShapeVee
//---------------------------------------------------------

class EndpointShapeVee : public EndpointShape
{
public:
	EndpointShapeVee(qreal lw, qreal sz) {
		sz *= lw;
		qreal dx = s_headHalfWidth;
		qreal dy = s_height;
		qreal t = 0.5*lw * sqrt(dx*dx+dy*dy) / dx;
		dx *= sz; dy *= sz;

		m_p[0] = QPointF(-dx,dy+t);
		m_p[1] = QPointF(0,t);
		m_p[2] = QPointF(dx, dy+t);

		m_length = 2*t;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.drawPolyline(m_p,3);
	}

	qreal penWidth(qreal lw) { return lw; }
	Qt::PenCapStyle capStyle() { return Qt::RoundCap; }

private:
	QPointF m_p[3];
	qreal m_length;

	static qreal s_headHalfWidth;
	static qreal s_height;
};

qreal EndpointShapeVee::s_headHalfWidth  = 3.5;
qreal EndpointShapeVee::s_height = 6.0;


//---------------------------------------------------------
// EndpointShapeCircle
//---------------------------------------------------------

class EndpointShapeCircle : public EndpointShape
{
public:
	EndpointShapeCircle(qreal lw, qreal sz) {
		sz *= lw;
		lw = penWidth(lw);
		m_r = sz*s_radius;
		m_y = m_r + 0.5*lw;
		m_length = 2*m_r+lw;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.drawEllipse(QPointF(0,m_y), m_r, m_r);
	}

private:
	qreal m_y;
	qreal m_r;
	qreal m_length;

	static qreal s_radius;
};

qreal EndpointShapeCircle::s_radius = 3.0;


//---------------------------------------------------------
// EndpointShapeBox
//---------------------------------------------------------

class EndpointShapeBox : public EndpointShape
{
public:
	EndpointShapeBox(qreal lw, qreal sz) {
		sz *= lw;
		lw = penWidth(lw);
		m_a = sz*s_a;
		m_y = 0.5*lw;
		m_length = 2*m_a+lw;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.drawRect(QRectF(-m_a, m_y, m_a+m_a, m_a+m_a));
	}

private:
	qreal m_y;
	qreal m_a;
	qreal m_length;

	static qreal s_a;
};

qreal EndpointShapeBox::s_a = 3.0;


//---------------------------------------------------------
// EndpointShapeRhomb
//---------------------------------------------------------

class EndpointShapeRhomb : public EndpointShape
{
public:
	EndpointShapeRhomb(qreal lw, qreal sz) {
		sz *= lw;
		lw = penWidth(lw);
		qreal dx = s_headHalfWidth;
		qreal dy = s_headHalfHeight;
		qreal t = 0.5*lw * sqrt(dx*dx+dy*dy) / dx;

		m_path.moveTo(0,t);
		m_path.lineTo(sz*dx, sz*dy+t);
		m_path.lineTo(0, sz*2*dy+t);
		m_path.lineTo(-sz*dx, sz*dy+t);
		m_path.closeSubpath();

		m_length = 2*sz*dy;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.drawPath(m_path);
	}

private:
	QPainterPath m_path;
	qreal m_length;

	static qreal s_headHalfWidth;
	static qreal s_headHalfHeight;
};

qreal EndpointShapeRhomb::s_headHalfWidth  = 3.0;
qreal EndpointShapeRhomb::s_headHalfHeight = 5.0;


//---------------------------------------------------------
// EndpointShapeTee
//---------------------------------------------------------

class EndpointShapeTee : public EndpointShape
{
public:
	EndpointShapeTee(qreal lw, qreal sz) {
		sz *= lw;
		lw = penWidth(lw);
		m_halfWidth = sz*s_halfWidth;
		m_height = sz*s_height;
		m_y = 0.5*lw;
		m_length = m_height+lw;
	}

	qreal length() {
		return m_length;
	}

	void draw(QPainter &painter) {
		painter.setBrush(painter.pen().color());
		painter.drawRect(QRectF(-m_halfWidth, m_y, m_halfWidth+m_halfWidth, m_height));
	}

private:
	qreal m_y;
	qreal m_height;
	qreal m_halfWidth;
	qreal m_length;

	static qreal s_halfWidth;
	static qreal s_height;
};

qreal EndpointShapeTee::s_halfWidth = 3.0;
qreal EndpointShapeTee::s_height = 1.5;


//---------------------------------------------------------
// EndpointShapeFactory
//---------------------------------------------------------

EndpointShape *EndpointShapeFactory::create(EndpointType type, qreal lw, qreal sz)
{
	switch(type)
	{
	case stTriangle:
	case stOTriangle:
		return new EndpointShapeTriangle(lw,sz);

	case stCircle:
	case stOCircle:
		return new EndpointShapeCircle(lw,sz);

	case stBox:
	case stOBox:
		return new EndpointShapeBox(lw,sz);

	case stRhomb:
	case stORhomb:
		return new EndpointShapeRhomb(lw,sz);

	case stTee:
		return new EndpointShapeTee(lw,sz);

	case stArrow:
		return new EndpointShapeArrow(lw,sz);

	case stVee:
		return new EndpointShapeVee(lw,sz);
	}

	return 0;
}

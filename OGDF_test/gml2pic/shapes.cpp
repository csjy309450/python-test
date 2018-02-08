/******************************************************************************
 * File:   shapes.cpp
 * Author: Carsten Gutwenger
 *
 * Defines and implements node shape classes.
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


#define _USE_MATH_DEFINES
#include <cmath>

#define M_SQRT3 1.73205080756887729353

#include "NodeShape.h"
#include <ogdf/basic/basic.h>
#include <limits>

using namespace ogdf;


//---------------------------------------------------------
// NodeShape
//---------------------------------------------------------

// containement for rectangular shape
bool NodeShape::contains(qreal x, qreal y, qreal lw, QPointF p)
{
	return (x-lw/2 <= p.x()) && (p.x() <= x+width()+lw/2) && (y-lw/2 <= p.y()) && (p.y() <= y+height()+lw/2);
}


// cutpoint for rectangular shape
QPointF NodeShape::cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
{
	qreal xmin = x - lw/2;
	qreal xmax = x + width() + lw/2;
	qreal ymin = y - lw/2;
	qreal ymax = y + height() + lw/2;;

	qreal dx = pt.x() - ps.x();
	qreal dy = pt.y() - ps.y();

	if(dy != 0) {
		// below
		if(pt.y() > ymax) {
			qreal t = (ymax-ps.y()) / dy;
			qreal x = ps.x() + t*dx;

			if(xmin <= x && x <= xmax)
				return QPointF(x,ymax);

		// above
		} else if(pt.y() < ymin) {
			qreal t = (ymin-ps.y()) / dy;
			qreal x = ps.x() + t*dx;

			if(xmin <= x && x <= xmax)
				return QPointF(x,ymin);

		}
	}

	if(dx != 0) {
		// right
		if(pt.x() > xmax) {
			qreal t = (xmax-ps.x()) / dx;
			qreal y = ps.y() + t*dy;

			if(ymin <= y && y <= ymax)
				return QPointF(xmax,y);

		// left
		} else if(pt.x() < xmin) {
			qreal t = (xmin-ps.x()) / dx;
			qreal y = ps.y() + t*dy;

			if(ymin <= y && y <= ymax)
				return QPointF(xmin,y);

		}
	}

	return pt;
}


bool NodeShape::inPolygon(QPointF *p, int n, QPointF q)
{
	bool result = rightOf(p[0],p[1],q);

	for(int i = 2; i < n; ++i)
		result = result && rightOf(p[i-1],p[i],q);

	return result && rightOf(p[n-1],p[0],q);
}


qreal NodeShape::cutpoint(QPointF ps, QPointF pt, QPointF qs, QPointF qt)
{
	qreal vx = pt.x() - ps.x();
	qreal vy = pt.y() - ps.y();
	qreal wx = qt.x() - qs.x();
	qreal wy = qt.y() - qs.y();

	qreal s = vy*wx-vx*wy;

	if(s == 0)
		return std::numeric_limits<qreal>::infinity();

	qreal dx = qs.x()-ps.x();
	qreal dy = qs.y()-ps.y();

	qreal t = (vx*dy - vy*dx) / s;
	qreal t1 = (vx != 0) ? (dx+t*wx)/vx : (dy+t*wy)/vy;

	if(t1 < 0 || 1 < t1)
		return std::numeric_limits<qreal>::infinity();

	return t;
}


QPointF NodeShape::cutpoint(QPointF *p, int n, QPointF ps, QPointF pt)
{
	qreal t = std::numeric_limits<qreal>::infinity();
	for(int i = 0; i < n; ++i)
		t = min(t, cutpoint(p[i],p[(i+1) % n],pt,ps));

	if(t == std::numeric_limits<qreal>::infinity())
		return ps;

	return pt + t*(ps-pt);
}


QPointF NodeShape::cutCircle(qreal mx, qreal my, qreal r, QPointF ps, QPointF pt)
{
	QPointF v = ps - pt;

	qreal dx = pt.x() - mx;
	qreal dy = pt.y() - my;
	qreal A = v.x()*v.x() + v.y()*v.y();
	qreal B = v.x() * dx + v.y() * dy;
	qreal C = dx*dx + dy*dy;

	if(A == 0) return ps;
	qreal radicand = (r*r - C + (B*B)/A) / A;

	if(radicand < 0) return ps;

	qreal sr = sqrt(radicand);
	qreal t = min( sr - B/A, -sr - B/A );

	return pt + t*v;
}


//---------------------------------------------------------
// NodeShapeRect
//---------------------------------------------------------

class NodeShapeRect : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeRect(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		painter.drawRect( QRectF(x,y,m_width,m_height) );
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		return QRectF( x+dm, y+dm, m_width-dm-dm, m_height-dm-dm );
	}
};


//---------------------------------------------------------
// NodeShapeRoundedRect
//---------------------------------------------------------

const double cRoundedRect = 1 - 0.5*M_SQRT2;

class NodeShapeRoundedRect : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeRoundedRect(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal radius = getRadius();
		painter.drawRoundedRect( QRectF(x,y,m_width,m_height), radius, radius);
	}

	QRectF bbLabel(qreal x, qreal y, qreal)
	{
		qreal radius = getRadius();
		qreal len = cRoundedRect * radius;
		return QRectF( x+len, y+len, m_width-len-len, m_height-len-len );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		if(NodeShape::contains(x,y,lw,p) == false)
			return false;

		qreal r = getRadius();
		if(p.x() < x+r) {
			if(p.y() < y+r)
				return inCircle(x+r,y+r,r+lw/2,p);
			else if(p.y() > y+m_height-r)
				return inCircle(x+r,y+m_height-r,r+lw/2,p);

		} else if(p.x() > x+m_width-r) {
			if(p.y() < y+r)
				return inCircle(x+m_width-r,y+r,r+lw/2,p);
			else if(p.y() > y+m_height-r)
				return inCircle(x+m_width-r,y+m_height-r,r+lw/2,p);
		}

		return true;
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF c = NodeShape::cutPoint(x,y,lw,ps,pt);

		qreal r = getRadius();
		if(c.x() < x+r) {
			if(c.y() < y+r)
				return cutCircle(x+r, y+r, r+lw/2, ps, pt);
			else if(c.y() > y+m_height-r)
				return cutCircle(x+r,y+m_height-r, r+lw/2, ps, pt);

		} else if(c.x() > x+m_width-r) {
			if(c.y() < y+r)
				return cutCircle(x+m_width-r, y+r, r+lw/2, ps, pt);
			else if(c.y() > y+m_height-r)
				return cutCircle(x+m_width-r,y+m_height-r, r+lw/2, ps, pt);
		}

		return c;
	}

private:
	qreal getRadius() const {
		return min(m_width,m_height)/6;
	}
};


//---------------------------------------------------------
// NodeShapeEllipse
//---------------------------------------------------------

class NodeShapeEllipse : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeEllipse(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		painter.drawEllipse( QRectF(x,y,m_width,m_height) );
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		return QRectF( x+dm, y+dm, m_width-dm-dm, m_height-dm-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		qreal a = 0.5 * (m_width+lw);
		qreal b = 0.5 * (m_height+lw);

		qreal mx = x + 0.5*m_width;
		qreal my = y + 0.5*m_height;

		qreal sx = (p.x()-mx) / a;
		qreal sy = (p.y()-my) / b;
		return ( sx*sx + sy*sy <= 0.25 );
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		qreal a = 0.5 * (m_width+lw);
		qreal b = 0.5 * (m_height+lw);

		if(a <= 0 || b <= 0)
			return NodeShape::cutPoint(x,y,lw,ps,pt);

		qreal mx = x + 0.5*m_width;
		qreal my = y + 0.5*m_height;

		QPointF v = ps - pt;

		qreal dx = pt.x() - mx;
		qreal dy = pt.y() - my;
		qreal A = (v.x()*v.x()) / (a*a) + (v.y()*v.y()) / (b*b);
		qreal B = (v.x() * dx) / (a*a) + (v.y() * dy) / (b*b);
		qreal C = (dx*dx)/(a*a) + (dy*dy)/(b*b);

		if(A == 0) return ps;
		qreal radicand = (1 - C + (B*B)/A) / A;

		if(radicand < 0) return ps;

		qreal sr = sqrt(radicand);
		qreal t = min( sr - B/A, -sr - B/A );

		return pt + t*v;
	}
};


//---------------------------------------------------------
// NodeShapeHexagon
//---------------------------------------------------------

const double cHexagon = M_SQRT3/6.0;

class NodeShapeHexagon : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeHexagon(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal len = getLen();

		QPainterPath p;
		p.moveTo( x+len,         y );
		p.lineTo( x+m_width-len, y );
		p.lineTo( x+m_width,     y+0.5*m_height );
		p.lineTo( x+m_width-len, y+m_height );
		p.lineTo( x+len,         y+m_height );
		p.lineTo( x,             y+0.5*m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		qreal len = getLen();
		return QRectF( x+len, y+dm, m_width-len-len, m_height-dm-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[6];
		makeOutline(points,x,y,lw);

		return inPolygon(points,6,p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[6];
		makeOutline(p,x,y,lw);

		return cutpoint(p,6,ps,pt);
	}

private:
	qreal getLen() const
	{
		return (m_height <= m_width) ? cHexagon * m_height : cHexagon*m_width;
	}

	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		qreal len = getLen();

		p[0].rx() = x + len - lw/2;
		p[0].ry() = y - lw/2;

		p[1].rx() = x + m_width - len + lw/2;
		p[1].ry() = y - lw/2;

		p[2].rx() = x + m_width + lw/2;
		p[2].ry() = y + 0.5*m_height;

		p[3].rx() = x + m_width - len + lw/2;
		p[3].ry() = y + m_height + lw/2;

		p[4].rx() = x + len - lw/2;
		p[4].ry() = y + m_height + lw/2;

		p[5].rx() = x - lw/2;
		p[5].ry() = y + 0.5*m_height;
	}
};


//---------------------------------------------------------
// NodeShapeTriangle
//---------------------------------------------------------

class NodeShapeTriangle : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeTriangle(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		QPainterPath p;
		p.moveTo( x+0.5*m_width, y );
		p.lineTo( x+m_width,     y+m_height );
		p.lineTo( x,             y+m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		return QRectF( x+0.25*m_width, y+0.5*m_height, 0.5*m_width, 0.5*m_height-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[3];
		makeOutline(points,x,y,lw);

		return rightOf(points[0], points[1], p) && rightOf(points[1], points[2], p) && rightOf(points[2], points[0], p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[3];
		makeOutline(p,x,y,lw);

		return cutpoint(p,3,ps,pt);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		p[0].rx() = x+0.5*m_width, y-lw/2;
		p[0].ry() = y-lw/2;

		p[1].rx() = x+m_width+lw/2;
		p[1].ry() = y+m_height+lw/2;

		p[2].rx() = x-lw/2;
		p[2].ry() = y+m_height+lw/2;
	}
};


//---------------------------------------------------------
// NodeShapeInvTriangle
//---------------------------------------------------------

class NodeShapeInvTriangle : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeInvTriangle(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		QPainterPath p;
		p.moveTo( x,             y );
		p.lineTo( x+m_width,     y );
		p.lineTo( x+0.5*m_width, y+m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		return QRectF( x+0.25*m_width, y+dm, 0.5*m_width, 0.5*m_height-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[3];
		makeOutline(points,x,y,lw);

		return rightOf(points[0], points[1], p) && rightOf(points[1], points[2], p) && rightOf(points[2], points[0], p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[3];
		makeOutline(p,x,y,lw);

		return cutpoint(p,3,ps,pt);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		p[0].rx() = x-lw/2;
		p[0].ry() = y-lw/2;

		p[1].rx() = x+m_width+lw/2;
		p[1].ry() = y-lw/2;

		p[2].rx() = x+0.5*m_width;
		p[2].ry() = y+m_height+lw/2;
	}
};


//---------------------------------------------------------
// NodeShapePentagon
//---------------------------------------------------------

	//// compute coordinates of pentagon in unit circle
	//const qreal cSqrt5 = sqrt(5.0);
	//const qreal cPenta_a  = 10.0 / sqrt(50.0+10*cSqrt5);
	//const qreal cPenta_ri = sqrt( (25+10*cSqrt5) / (50+10*cSqrt5) );
	//const qreal cPenta_h  = 1+cPenta_ri;
	//const qreal cPenta_d  = 0.5*(1+cSqrt5)*cPenta_a;
	//const qreal cPenta_h1 = sqrt(1 - 0.25*cPenta_d*cPenta_d);
	//const qreal cPenta_t  = (cPenta_d-cPenta_a)*(1-cPenta_h1)/cPenta_d;

	//QPointF uPenta[5];
	//uPenta[0] = QPointF(0.5*cPenta_d, 0);
	//uPenta[1] = QPointF(cPenta_d, 1-cPenta_h1);
	//uPenta[2] = QPointF(0.5*(cPenta_d+cPenta_a), 1+cPenta_ri);
	//uPenta[3] = QPointF(0.5*(cPenta_d-cPenta_a), 1+cPenta_ri);
	//uPenta[4] = QPointF(0, 1-cPenta_h1);


const qreal uPenta[5][2] = {
	0.95105651629515364, 0,
	1.9021130325903073,  0.69098300562505277,
	1.5388417685876268,  1.809016994374947,
	0.36327126400268051, 1.8090169943749475,
	0,                   0.69098300562505277
};

const qreal cPenta_d  = 1.9021130325903073;
const qreal cPenta_h  = 1.8090169943749475;
const qreal cPenta_a  = 1.1755705045849463;
const qreal cPenta_ri = 0.80901699437494745;
const qreal cPenta_h1 = 0.30901699437494723;
const qreal cPenta_t  = 0.26393202250021036;

class NodeShapePentagon : public NodeShape
{
	qreal m_sx;
	qreal m_sy;

public:
	NodeShapePentagon(qreal w, qreal h) {
		m_sx = w / cPenta_d;
		m_sy = h / cPenta_h;
	}

	qreal width () { return m_sx*cPenta_d; }
	qreal height() { return m_sy*(1+cPenta_ri); }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		QPainterPath p;
		p.moveTo( x + m_sx*uPenta[0][0], y + m_sy*uPenta[0][1] );
		for(int i = 1; i < 5; ++i)
			p.lineTo(x + m_sx*uPenta[i][0], y + m_sy*uPenta[i][1]);
		p.closeSubpath();
		painter.drawPath(p);
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		return QRectF( x+0.5*m_sx*(cPenta_d-cPenta_a), y+m_sy*(1-cPenta_h1-cPenta_t),
			m_sx*cPenta_a, m_sy*(cPenta_ri+cPenta_h1+cPenta_t)-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[5];
		makeOutline(points,x,y,lw);

		return inPolygon(points,5,p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[5];
		makeOutline(p,x,y,lw);

		return cutpoint(p,5,ps,pt);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		p[0].rx() = x + m_sx*uPenta[0][0];
		p[0].ry() = y + m_sy*uPenta[0][1] - lw/2;

		p[1].rx() = x + m_sx*uPenta[1][0] + lw/2;
		p[1].ry() = y + m_sy*uPenta[1][1];

		p[2].rx() = x + m_sx*uPenta[2][0] + lw/2;
		p[2].ry() = y + m_sy*uPenta[2][1] + lw/2;

		p[3].rx() = x + m_sx*uPenta[3][0] - lw/2;
		p[3].ry() = y + m_sy*uPenta[3][1] + lw/2;

		p[4].rx() = x + m_sx*uPenta[4][0] - lw/2;
		p[4].ry() = y + m_sy*uPenta[4][1];

	}
};


//---------------------------------------------------------
// NodeShapeOctagon
//---------------------------------------------------------

class NodeShapeOctagon : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeOctagon(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal a = get_a();
		QPainterPath p;
		p.moveTo( x+a,         y );
		p.lineTo( x+m_width-a, y );
		p.lineTo( x+m_width,   y+a );
		p.lineTo( x+m_width,   y+m_height-a );
		p.lineTo( x+m_width-a, y+m_height );
		p.lineTo( x+a,         y+m_height );
		p.lineTo( x,           y+m_height-a );
		p.lineTo( x,           y+a );
		p.closeSubpath();
		painter.drawPath(p);
	}

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		qreal a = get_a();
		return QRectF( x+a, y+dm, m_width-a-a, m_height-dm-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[8];
		makeOutline(points,x,y,lw);

		return inPolygon(points,8,p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[8];
		makeOutline(p,x,y,lw);

		return cutpoint(p,8,ps,pt);
	}

private:
	qreal get_a() const {
		return min(m_width,m_height) / (2+M_SQRT2);
	}

	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		qreal a = get_a();

		p[0].rx() = x + a - lw/2;
		p[0].ry() = y - lw/2;

		p[1].rx() = x + m_width - a + lw/2;
		p[1].ry() = y - lw/2;

		p[2].rx() = x + m_width + lw/2;
		p[2].ry() = y + a - lw/2;

		p[3].rx() = x + m_width + lw/2;
		p[3].ry() = y + m_height - a + lw/2;

		p[4].rx() = x + m_width - a + lw/2;
		p[4].ry() = y + m_height + lw/2;

		p[5].rx() = x + a - lw/2;
		p[5].ry() = y + m_height + lw/2;

		p[6].rx() = x - lw/2;
		p[6].ry() = y + m_height - a + lw/2;

		p[7].rx() = x - lw/2;
		p[7].ry() = y + a - lw/2;
	}
};


//---------------------------------------------------------
// NodeShapeRhomb
//---------------------------------------------------------

class NodeShapeRhomb : public NodeShape
{
	qreal m_width;
	qreal m_height;

public:
	NodeShapeRhomb(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		QPainterPath p;
		p.moveTo( x+0.5*m_width, y );
		p.lineTo( x+m_width,     y+0.5*m_height );
		p.lineTo( x+0.5*m_width, y+m_height );
		p.lineTo( x,             y+0.5*m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

	QRectF bbLabel(qreal x, qreal y, qreal)
	{
		return QRectF( x+0.25*m_width, y+0.25*m_height, 0.5*m_width, 0.5*m_height );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[4];
		makeOutline(points,x,y,lw);

		return inPolygon(points,4,p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[4];
		makeOutline(p,x,y,lw);

		return cutpoint(p,4,ps,pt);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		p[0].rx() = x + 0.5*m_width;
		p[0].ry() = y - lw/2;

		p[1].rx() = x + m_width + lw/2;
		p[1].ry() = y + 0.5*m_height;

		p[2].rx() = x + 0.5*m_width;
		p[2].ry() = y + m_height + lw/2;

		p[3].rx() = x - lw/2;
		p[3].ry() = y + 0.5*m_height;
	}
};


//---------------------------------------------------------
// NodeShapeTrapezeParallelogram
//---------------------------------------------------------

const double cTan75 = tan(M_PI * (75.0 / 180.0));

class NodeShapeTrapezeParallelogram : public NodeShape
{
protected:
	qreal m_width;
	qreal m_height;

	qreal getLen() const
	{
		qreal len = m_height / cTan75;
		if(m_width - 2*len < 0.5*m_height)
			len = 0.25*m_width;
		return len;
	}

public:
	NodeShapeTrapezeParallelogram(qreal w, qreal h) {
		m_width = w;
		m_height = h;
	}

	qreal width () { return m_width;  }
	qreal height() { return m_height; }

	QRectF bbLabel(qreal x, qreal y, qreal dm)
	{
		qreal len = getLen();
		return QRectF( x+len, y+dm, m_width-len-len, m_height-dm-dm );
	}

	bool contains(qreal x, qreal y, qreal lw, QPointF p)
	{
		QPointF points[4];
		makeOutline(points,x,y,lw);

		return inPolygon(points,4,p);
	}

	QPointF cutPoint(qreal x, qreal y, qreal lw, QPointF ps, QPointF pt)
	{
		QPointF p[4];
		makeOutline(p,x,y,lw);

		return cutpoint(p,4,ps,pt);
	}

private:
	virtual void makeOutline(QPointF *p, qreal x, qreal y, qreal lw) = 0;
};


//---------------------------------------------------------
// NodeShapeTrapeze
//---------------------------------------------------------

class NodeShapeTrapeze : public NodeShapeTrapezeParallelogram
{
public:
	NodeShapeTrapeze(qreal w, qreal h) : NodeShapeTrapezeParallelogram(w,h) { }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal len = getLen();

		QPainterPath p;
		p.moveTo( x+len,         y );
		p.lineTo( x+m_width-len, y );
		p.lineTo( x+m_width,     y+m_height );
		p.lineTo( x,             y+m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		qreal len = getLen();

		p[0].rx() = x+len-lw/2;
		p[0].ry() = y-lw/2;
		p[1].rx() = x+m_width-len+lw/2;
		p[1].ry() = y-lw/2;
		p[2].rx() = x+m_width+lw/2;
		p[2].ry() = y+m_height+lw/2;
		p[3].rx() = x-lw/2;
		p[3].ry() = y+m_height+lw/2;
	}
};


//---------------------------------------------------------
// NodeShapeInvTrapeze
//---------------------------------------------------------

class NodeShapeInvTrapeze : public NodeShapeTrapezeParallelogram
{
public:
	NodeShapeInvTrapeze(qreal w, qreal h) : NodeShapeTrapezeParallelogram(w,h) { }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal len = getLen();

		QPainterPath p;
		p.moveTo( x,             y );
		p.lineTo( x+m_width,     y );
		p.lineTo( x+m_width-len, y+m_height );
		p.lineTo( x+len,         y+m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		qreal len = getLen();

		p[0].rx() = x-lw/2;
		p[0].ry() = y-lw/2;
		p[1].rx() = x+m_width+lw/2;
		p[1].ry() = y-lw/2;
		p[2].rx() = x+m_width-len+lw/2;
		p[2].ry() = y+m_height+lw/2;
		p[3].rx() = x+len-lw/2;
		p[3].ry() = y+m_height+lw/2;
	}
};


//---------------------------------------------------------
// NodeShapeParallelogram
//---------------------------------------------------------

class NodeShapeParallelogram : public NodeShapeTrapezeParallelogram
{
public:
	NodeShapeParallelogram(qreal w, qreal h) : NodeShapeTrapezeParallelogram(w,h) { }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal len = getLen();

		QPainterPath p;
		p.moveTo( x+len,         y );
		p.lineTo( x+m_width,     y );
		p.lineTo( x+m_width-len, y+m_height );
		p.lineTo( x,             y+m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		qreal len = getLen();

		p[0].rx() = x+len-lw/2;
		p[0].ry() = y-lw/2;
		p[1].rx() = x+m_width+lw/2;
		p[1].ry() = y-lw/2;
		p[2].rx() = x+m_width-len+lw/2;
		p[2].ry() = y+m_height+lw/2;
		p[3].rx() = x-lw/2;
		p[3].ry() = y+m_height+lw/2;
	}
};


//---------------------------------------------------------
// NodeShapeInvParallelogram
//---------------------------------------------------------

class NodeShapeInvParallelogram : public NodeShapeTrapezeParallelogram
{
public:
	NodeShapeInvParallelogram(qreal w, qreal h) : NodeShapeTrapezeParallelogram(w,h) { }

	void draw(QPainter &painter, qreal x, qreal y)
	{
		qreal len = getLen();

		QPainterPath p;
		p.moveTo( x,             y );
		p.lineTo( x+m_width-len, y );
		p.lineTo( x+m_width,     y+m_height );
		p.lineTo( x+len,         y+m_height );
		p.closeSubpath();
		painter.drawPath(p);
	}

private:
	void makeOutline(QPointF *p, qreal x, qreal y, qreal lw)
	{
		qreal len = getLen();

		p[0].rx() = x-lw/2;
		p[0].ry() = y-lw/2;
		p[1].rx() = x+m_width-len+lw/2;
		p[1].ry() = y-lw/2;
		p[2].rx() = x+m_width+lw/2;
		p[2].ry() = y+m_height+lw/2;
		p[3].rx() = x+len-lw/2;
		p[3].ry() = y+m_height+lw/2;
	}
};


//---------------------------------------------------------
// NodeShapeFactory
//---------------------------------------------------------

NodeShape *NodeShapeFactory::create(Shape shape, qreal w, qreal h)
{
	switch(shape)
	{
	case shRect:
		return new NodeShapeRect(w,h);

	case shRoundedRect:
		return new NodeShapeRoundedRect(w,h);

	case shEllipse:
		return new NodeShapeEllipse(w,h);

	case shTriangle:
		return new NodeShapeTriangle(w,h);

	case shPentagon:
		return new NodeShapePentagon(w,h);

	case shHexagon:
		return new NodeShapeHexagon(w,h);

	case shOctagon:
		return new NodeShapeOctagon(w,h);

	case shRhomb:
		return new NodeShapeRhomb(w,h);

	case shTrapeze:
		return new NodeShapeTrapeze(w,h);

	case shParallelogram:
		return new NodeShapeParallelogram(w,h);

	case shInvTriangle:
		return new NodeShapeInvTriangle(w,h);

	case shInvTrapeze:
		return new NodeShapeInvTrapeze(w,h);

	case shInvParallelogram:
		return new NodeShapeInvParallelogram(w,h);
	}

	// should not reach this point!
	return new NodeShapeRect(w,h);  // last resort
}

/******************************************************************************
 * File:   GraphProcessor.cpp
 * Author: Carsten Gutwenger
 *
 * Implements some helper functions for drawing graphs.
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

#include "GraphProcessor.h"


using namespace ogdf;


void GraphProcessor::normalize(List<QPointF> &dpl)
{
	if (dpl.empty()) return;
	ListIterator<QPointF> iter, next;
	for (iter = next = dpl.begin(), ++next; next.valid() && (dpl.size() > 2); ++next) {
		if (*iter == *next) {
			dpl.del(next);
			next = iter;
		} else
			iter = next;
	}
}


QPointF *GraphProcessor::newPointArray(const List<QPointF> &dpl, int &head, int &tail)
{
	head = 0;
	tail = dpl.size()-1;
	QPointF *points = new QPointF[dpl.size()];

	ListConstIterator<QPointF> it;
	int i = 0;
	for(it = dpl.begin(); it.valid(); ++it, ++i)
		points[i] = *it;

	return points;
}


// creates a new point array initialized to the points in polyline dpl
// sets head to the first and tail to the last index in the created array
QPointF *GraphProcessor::newPointArray(const DPolyline &dpl, int &head, int &tail)
{
	head = 0;
	tail = dpl.size()-1;
	QPointF *points = new QPointF[dpl.size()];

	ListConstIterator<DPoint> it;
	int i = 0;
	for(it = dpl.begin(); it.valid(); ++it, ++i) {
		points[i].setX((*it).m_x);
		points[i].setY((*it).m_y);
	}

	return points;
}


QPainterPath GraphProcessor::quadraticBezierPath(const QPointF *points, int n, qreal curvedLength)
{
	QPainterPath path;

	path.moveTo(points[0]);
	if(n == 2) {
		path.lineTo(points[1]);

	} else {
		// first segment
		QPointF diff = points[1] - points[0];
		qreal length = euclideanLength(diff);
		if(length > curvedLength) {
			QPointF v = curvedLength/length * diff;
			path.lineTo(points[1] - v);
		}

		// middle segments
		int i = 1;
		while(i < n-2) {
			diff = points[i+1] - points[i];
			length = euclideanLength(diff);
			if(length <= 2*curvedLength) {
				path.quadTo(points[i], points[i] + 0.5*diff);
			} else {
				QPointF v = curvedLength/length * diff;
				path.quadTo(points[i], points[i] + v);
				path.lineTo(points[i+1] - v);
			}
			i++;
		}

		// last segment
		diff = points[i+1] - points[i];
		length = euclideanLength(diff);
		if(length <= curvedLength) {
			path.quadTo(points[i], points[n-1]);
		} else {
			QPointF v = curvedLength/length * diff;
			path.quadTo(points[i], points[i] + v);
			path.lineTo(points[n-1]);
		}
	}

	return path;
}


QPainterPath GraphProcessor::cubicBezierPath(const QPointF *points, int n, qreal curvedLength)
{
	QPainterPath path;

	path.moveTo(points[0]);
	if(n == 2) {
		path.lineTo(points[1]);

	} else {
		// first segment
		QPointF diff = points[1] - points[0];
		qreal length = euclideanLength(diff);
		if(length > curvedLength) {
			QPointF v = curvedLength/length * diff;
			path.lineTo(points[1] - v);
		}

		// middle segments
		int i = 1;
		while(i < n-2) {
			diff = points[i+1] - points[i];
			length = euclideanLength(diff);
			if(length <= 2*curvedLength) {
				path.cubicTo(points[i], points[i], points[i] + 0.5*diff);
			} else {
				QPointF v = curvedLength/length * diff;
				path.cubicTo(points[i], points[i], points[i] + v);
				path.lineTo(points[i+1] - v);
			}
			i++;
		}

		// last segment
		diff = points[i+1] - points[i];
		length = euclideanLength(diff);
		if(length <= curvedLength) {
			path.cubicTo(points[i], points[i], points[n-1]);
		} else {
			QPointF v = curvedLength/length * diff;
			path.cubicTo(points[i], points[i], points[i] + v);
			path.lineTo(points[n-1]);
		}
	}

	return path;
}


QPainterPath GraphProcessor::circularArcPath(const QPointF *points, int n, qreal curvedLength)
{
	QPainterPath path;
	const qreal toDegree = 180 / M_PI;

	path.moveTo(points[0]);
	if(n == 2) {
		path.lineTo(points[1]);

	} else {
		for(int i = 1; i < n-1; ++i)
		{
			// process bend point at points[i]

			qreal len = curvedLength;

			// line segment from points[i] to points[i-1]
			QPointF n_1 = points[i-1] - points[i];
			qreal length_1 = euclideanLength(n_1);
			len = min(len, (i == 1) ? length_1 : 0.5*length_1);
			n_1 /= length_1;

			// line segment from points[i] to points[i+1]
			QPointF n_2 = points[i+1] - points[i];
			qreal length_2 = euclideanLength(n_2);
			len = min(len, (i == n-2) ? length_2 : 0.5*length_2);
			n_2 /= length_2;

			// angle between n_1 and n_2
			qreal alpha = acos(dotProduct(n_1,n_2));

			// angle bisector for alpha
			QPointF v = n_1 + n_2;
			::normalize(v);

			// radius of circle
			qreal radius = len * tan(alpha/2);

			// center of circle
			QPointF M = points[i] + sqrt(len*len+radius*radius) * v;

			// bounding box of circle
			QRectF bb(M.x()-radius, M.y()-radius, 2*radius, 2*radius);

			// right or left bend?
			bool rightBend = ( -n_2.y()*n_1.x() + n_1.y()*n_2.x() > 0 );

			// compute angle for specifying arc
			qreal sweepLength = 180 - toDegree * alpha;
			if(rightBend)
				sweepLength = -sweepLength;

			qreal ny = (rightBend) ? -n_1.y() : n_1.y();
			qreal nx = (rightBend) ? n_1.x() : -n_1.x();
			qreal startAngle = toDegree * ( (nx > 0) ? -acos(ny) : acos(ny) );

			path.arcTo(bb, startAngle, sweepLength);
		}
		path.lineTo(points[n-1]);
	}

	return path;
}





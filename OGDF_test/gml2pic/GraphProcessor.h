/******************************************************************************
 * File:   GraphProcessor.h
 * Author: Carsten Gutwenger
 *
 * Defines class GraphProcessor, which defines the interface for loading and
 * rendering graphs.
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


#ifndef _GRAPH_PROCESSOR_H
#define _GRAPH_PROCESSOR_H


#include <QString>
#include <QPainter>
#include "gml2pic.h"
#include <ogdf/basic/geometry.h>



/******************************************************************************
 *                              CLASS GraphProcessor
 *
 * Base class for classes responsible for processing (= reading and drawing)
 * graphs.
 ******************************************************************************/

class GraphProcessor
{
public:
	GraphProcessor() { }
	virtual ~GraphProcessor() { }

	// reads the graph from file filename
	virtual bool read(const QString &filename) = 0;
	// draws the graph with the painter
	virtual void draw(QPainter &painter) = 0;

	// returns the number of nodes in the loaded graph
	virtual int numberOfNodes() const = 0;
	// returns the number of edges in the loaded graph
	virtual int numberOfEdges() const = 0;
	// returns the number of clusters in the loaded graph
	virtual int numberOfClusters() const = 0;

	// returns the bounding box of the loaded graph
	virtual QRectF boundingBox() const = 0;

protected:
	static void normalize(List<QPointF> &dpl);

	static QPointF *newPointArray(const List<QPointF> &dpl, int &head, int &tail);
	static QPointF *newPointArray(const DPolyline &dpl, int &head, int &tail);

	// draws polyline with quadratic Beziers (at bends)
	static QPainterPath quadraticBezierPath(const QPointF *points, int n, qreal curvedLength);
	// draws polyline with cubic Beziers (at bends)
	static QPainterPath cubicBezierPath(const QPointF *points, int n, qreal curvedLength);
	// draws polyline with circular ars (at bends)
	static QPainterPath circularArcPath(const QPointF *points, int n, qreal curvedLength);
};


#endif

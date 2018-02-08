/******************************************************************************
 * File:   GMLProcessor.h
 * Author: Carsten Gutwenger
 *
 * Defines class GMLProcessor, which is responsible for loading and rendering
 * graphs from GML files.
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


#ifndef _GML_PROCESSOR_H
#define _GML_PROCESSOR_H


#include "GraphProcessor.h"
#include <ogdf/cluster/ClusterGraphAttributes.h>
#include "NodeShape.h"

using namespace ogdf;


/******************************************************************************
 *                              CLASS GMLProcessor
 *
 * Reads graphs from GML files.
 ******************************************************************************/

class GMLProcessor : public GraphProcessor
{
	Graph                  m_graph;  // OGDF graph
	ClusterGraph           m_cg;     // OGDF cluster graph
	ClusterGraphAttributes m_cga;    // OGDF cluster graph attributes
	const Options &m_options;        // reference to gml2pic options

public:
	// creates a GML processor
	GMLProcessor(const Options &opt) :
		m_cg(m_graph),
		m_cga(m_cg,
			GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
			GraphAttributes::nodeColor    | GraphAttributes::edgeArrow    |
			GraphAttributes::edgeColor    | GraphAttributes::edgeStyle    |
			GraphAttributes::nodeStyle    | GraphAttributes::nodeTemplate |
			GraphAttributes::nodeLabel),
		m_options(opt)
	{
	}

	// reads the graph from file filename
	bool read(const QString &filename);

	// draws the graph with the painter
	void draw(QPainter &painter);

	// returns the number of nodes in the loaded graph
	int numberOfNodes() const { return m_graph.numberOfNodes(); }
	// returns the number of edges in the loaded graph
	int numberOfEdges() const { return m_graph.numberOfEdges(); }
	// returns the number of clusters in the loaded graph
	int numberOfClusters() const { return m_cg.numberOfClusters(); }

	// returns the bounding box of the loaded graph
	QRectF boundingBox() const;

private:
	// retrieve text and formatting info from string representation
	static void labelFromString(const char *attrStr, QColor &myTextColor, QFont &myFont, int &myAlign, QString &myLabel);

	void getLevelOrderClusters(const ClusterGraph &cg, SListPure<cluster> &clusters);
	void normalizeEdge(const NodeArray<NodeShape*> &vShape, 
		QPointF *points, int &head, int &tail, node src, node tgt);
};


#endif

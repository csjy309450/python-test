/******************************************************************************
 * File:   OGMLProcessor.h
 * Author: Carsten Gutwenger
 *
 * Defines class GMLProcessor, which is responsible for loading and rendering
 * graphs from OGML files.
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


#ifndef _OGML_PROCESSOR_H
#define _OGML_PROCESSOR_H


#include <QIODevice>
#include <QXmlStreamReader>
#include "GraphProcessor.h"
#include "OGMLGraph.h"


/******************************************************************************
 *                              CLASS OGMLProcessor
 *
 * Reads graphs from OGML files.
 ******************************************************************************/

class OGMLProcessor : public GraphProcessor
{
	QXmlStreamReader m_xml;    // XML parser
	OGMLGraph        m_graph;  // OGML graph

	QHash<QString,IntType> m_table;

public:
	// creates an OGML processor
	OGMLProcessor(const Options &opt);

	// reads the graph from file filename
	bool read(const QString &filename);

	// draws the graph with the painter
	void draw(QPainter &painter);

	// returns the number of nodes in the loaded graph
	int numberOfNodes()    const { return m_graph.numberOfNodes();    }
	// returns the number of edges in the loaded graph
	int numberOfEdges()    const { return m_graph.numberOfEdges();    }
	// returns the number of clusters in the loaded graph
	int numberOfClusters() const { return m_graph.numberOfClusters(); }

	// returns the bounding box of the loaded graph
	QRectF boundingBox() const { return m_graph.boundingBox(); }

private:
	enum PredefKeys {
		widthKey, heightKey, shapeKey, colorKey, patternKey, patternColorKey, typeKey, sizeKey,
		horizontalAlignKey, verticalAlignKey, decorationKey, transformKey,
		familyKey, variantKey, weightKey, stretchKey, styleKey	
	};

	void readGraphTag();
	void readLayoutTag();
	void readStructureTag();
	void readNodeTag();
	void readEdgeTag();
	void readLabelTag(OGMLGraph::Node *parentNode = 0);
	void readStylesTag();
	void readNodeStyleTag();
	void readEdgeStyleTag();
	void readLabelStyleTag();

	void normalizeEdge(QPointF *points, int &head, int &tail, OGMLGraph::Node *src, OGMLGraph::Node *tgt);

	void outputError();
};


#endif

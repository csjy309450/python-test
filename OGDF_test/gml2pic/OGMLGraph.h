/******************************************************************************
 * File:   OGMLGraph.h
 * Author: Carsten Gutwenger
 *
 * Defines class OGMLGraph, which represents a graph layout stored in the
 * OGML file format.
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


#ifndef _OGML_GRAPH_H
#define _OGML_GRAPH_H


#include "gml2pic.h"
#include "NodeShape.h"
#include "EndpointShape.h"
#include <QRectF>
#include <QHash>
#include <ogdf/basic/List.h>

#ifdef __GNUC__
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#include <x86intrin.h>
#endif
#endif

using namespace ogdf;

	
/******************************************************************************
 *                               auxiliary structs
 ******************************************************************************/

// Wrapper for int with default value (-1)
struct IntType 
{
	IntType() { m_int = -1; }
	IntType(int e) { m_int = e; }

	operator int() { return m_int; }

	int m_int;
};



/******************************************************************************
 *                               CLASS OGMLGraph
 *
 * Representation for graphs stored in OGML format.
 ******************************************************************************/

class OGMLGraph
{
	enum Enums { eString, eLineType, eShapeType, eEndpointType, ePatternType, eHAlignType, eVAlignType,
		eDecorationType, eCapitalizationType, eFontStretchType, eFontWeightType, eFontStyleType };

	struct EnumString
	{
		EnumString() { m_enum = eString; }
		EnumString(const QString &str) : m_string(str) { m_enum = eString; }
		EnumString(Enums e, const QString &str) : m_string(str) { m_enum = e; }

		bool operator==(const EnumString &e) const {
			return m_enum == e.m_enum && m_string == e.m_string;
		}

		Enums   m_enum;
		QString m_string;
	};


	typedef QHash<EnumString,IntType>::ConstIterator StringHashIter;

	// hash function for EnumString
	friend uint qHash(const OGMLGraph::EnumString &s)
	{
		uint hs = qHash(s.m_string);
		uint he = s.m_enum;
		_rotl(he,16);
		return hs ^ he;
	}

public:
	enum DecorationType { dtNone, dtUnderline, dtOverline, dtLineThrough };

	/******************************************************************
	 *                      NESTED CLASS Node
	 * Representation for nodes.
	 ******************************************************************/

	struct Node
	{
		Node() {
			m_next = 0;
			m_isCluster = false;

			m_location         = QPointF(0,0);
			m_pattern          = Qt::SolidPattern;
			m_fillColor        = Qt::white;
			m_fillPatternColor = Qt::black;
			m_lineType         = Qt::SolidLine;
			m_lineColor        = Qt::black;
			m_lineWidth        = 1.0;
			m_nodeShape        = 0;
		}

		~Node() {
			delete m_nodeShape;
		}

		QPointF center() const {
			return (m_nodeShape != 0) ? QPointF(m_location.x()+0.5*m_nodeShape->width(), m_location.y()+0.5*m_nodeShape->height()) : m_location;
		}

		QRectF bbLabel(qreal dm) const {
			return m_nodeShape->bbLabel(m_location.x(), m_location.y(), dm);
		}

		bool contains(QPointF p) {
			return (m_nodeShape != 0) ? m_nodeShape->contains(m_location.x(), m_location.y(), m_lineWidth, p) : (p == m_location);
		}

		QPointF cutPoint(QPointF ps, QPointF pt) {
			return (m_nodeShape != 0) ? m_nodeShape->cutPoint(m_location.x(), m_location.y(), m_lineWidth, ps, pt) : m_location; 
		}

		Node          *m_next;                // next node in list of all nodes
		bool           m_isCluster;           // is this node a cluster?

		QPointF        m_location;            // position
		Qt::BrushStyle m_pattern;             // fill pattern
		QColor         m_fillColor;           // fill color
		QColor         m_fillPatternColor;    // fill pattern color
		Qt::PenStyle   m_lineType;            // stroke pattern
		QColor         m_lineColor;           // stroke color
		qreal          m_lineWidth;           // stroke width
		NodeShape     *m_nodeShape;           // node shape
	};

	/******************************************************************
	 *                      NESTED CLASS Edge
	 * Representation for edges.
	 ******************************************************************/

	struct Edge
	{
		Edge() {
			m_src = m_tgt = 0;

			m_lineType  = Qt::SolidLine;
			m_lineColor = Qt::black;
			m_lineWidth = 1.0;
			m_srcType   = stNone;
			m_tgtType   = stArrow;
			m_srcColor  = Qt::black;
			m_tgtColor  = Qt::black;
			m_srcSize   = 1.0;
			m_tgtSize   = 1.0;
		}

		Node         *m_src;         // source node
		Node         *m_tgt;         // target node

		List<QPointF> m_points;      // bend points

		Qt::PenStyle  m_lineType;    // stroke pattern
		QColor        m_lineColor;   // stroke color
		qreal         m_lineWidth;   // stroke width
		EndpointType  m_srcType;     // endpoint shape at soure node
		EndpointType  m_tgtType;     // endpoint shape at target node
		QColor        m_srcColor;    // color for endpoint shape at soure node
		QColor        m_tgtColor;    // color for endpoint shape at target node
		qreal         m_srcSize;     // size for endpoint shape at soure node
		qreal         m_tgtSize;     // size for endpoint shape at target node
	};

	/******************************************************************
	 *                      NESTED CLASS Label
	 * Representation for labels.
	 ******************************************************************/

	struct Label
	{
		Label() {
			m_parentNode = 0;

			m_location       = QPointF(0,0);
			m_width          = 0;
			m_height         = 0;
			m_alignment      = 0;
			m_stretch        = QFont::Unstretched;
			m_weight         = QFont::Normal;
			m_capitalization = QFont::MixedCase;
			m_style          = 0;
			m_underline      = 0;
			m_overline       = 0;
			m_strikeOut      = 0;
		}

		QRectF bb() const {
			return QRectF(m_location, QSizeF(m_width,m_height));
		}

		Node          *m_parentNode;   // parent node (if node label), otherwise 0

		QString        m_content;      // label text
		QPointF        m_location;     // label position
		qreal          m_width;        // label width
		qreal          m_height;       // label height
		int            m_alignment;    // alignment
		StringHashIter m_family;       // font family
		qreal          m_pointSize;    // font point size
		struct {
			uint m_stretch        : 8;  // font stretch
			uint m_weight         : 8;  // font weight
			uint m_capitalization : 3;  // capitalization
			uint m_style          : 2;  // font style
			uint m_underline      : 1;  // underline?
			uint m_overline       : 1;  // overline?
			uint m_strikeOut      : 1;  // strike-through?
		};
		QColor         m_color;        // label color
	};

	// wrapper class for pointer
	template<class T> struct Pointer
	{
		Pointer() { p = 0; }
		T *p;
	};


	// creates an empty OGML graph
	OGMLGraph(const Options &opt);

	~OGMLGraph();

	// converts string to node shape type
	Shape toShape(const QString &str);

	// converts string to endpoint shape type
	EndpointType toEndpointType(const QString &str);

	// converts string to Qt brush style
	Qt::BrushStyle toPattern(const QString &str);

	// converts string to Qt pen style
	Qt::PenStyle toLineType(const QString &str);

	// converts string to horizontal alignment flags
	Qt::AlignmentFlag toHAlign(const QString &str);

	// converts string to vertical alignment flags
	Qt::AlignmentFlag toVAlign(const QString &str);

	// converts string to decoration type
	DecorationType toDecorationType(const QString &str);

	// converts string to Qt font capitalization type
	QFont::Capitalization toCapitalizationType(const QString &str);

	// converts string to Qt font stretch
	uint toFontStretch(const QString &str);

	// converts string to Qt font weight
	uint toFontWeight(const QString &str);

	// converts string to Qt font style
	uint toFontStyle(const QString &str);

	// insert string into hash table
	StringHashIter hashString(const QString &str) {
		return m_stringTable.insert(str,0);
	}

	// creates a new node for given ID (if not yet contained)
	// makes this node the last node in the list of all nodes
	Node  *createNode(const QString &id);

	// returns node for given ID; creates it if necessary
	Node  *getNode(const QString &id);

	// returns edge for given ID; creates it if necessary
	Edge  *getEdge(const QString &id);

	// returns label for given ID; creates it if necessary
	Label *getLabel(const QString &id);

	// returns the number of nodes in the graph
	int numberOfNodes() const {
		return m_numNodes;
	}

	// returns the number of edges in the graph
	int numberOfEdges() const {
		return m_numEdges;
	}

	// returns the number of clusters in the graph
	int numberOfClusters() const {
		return m_numClusters;
	}

	// returns the first node in the list of all nodes
	Node *firstNode() const {
		return m_firstNode;
	}

	// returns iterator to begin of node hash table
	QHashIterator<QString,Pointer<Node>  > beginNodes()  const { return QHashIterator<QString,Pointer<Node>  >(m_nodeTable);  }
	// returns iterator to begin of edge hash table
	QHashIterator<QString,Pointer<Edge>  > beginEdges()  const { return QHashIterator<QString,Pointer<Edge>  >(m_edgeTable);  }
	// returns iterator to begin of label hash table
	QHashIterator<QString,Pointer<Label> > beginLabels() const { return QHashIterator<QString,Pointer<Label> >(m_labelTable); }

	// returns the bounding box of the layout
	QRectF boundingBox() const;

	// mark node as cluster node
	void setNodeIsCluster(Node *n)
	{
		if(n->m_isCluster == false) {
			n->m_isCluster = true;
			--m_numNodes;
			++m_numClusters;
		}
	}

	// returns a reference to gml2pic options
	const Options &options() const { return m_options; }

private:
	int m_numNodes;     // number of nodes in the graph
	int m_numEdges;     // number of edges in the graph
	int m_numClusters;  // number of clusters in the graph

	QHash<EnumString,IntType> m_stringTable;  // common hash tablefor strings

	QHash<QString,Pointer<Node> >  m_nodeTable;  // hash table for nodes
	QHash<QString,Pointer<Edge> >  m_edgeTable;  // hash table for edges
	QHash<QString,Pointer<Label> > m_labelTable; // hash table for labels

	// node list is sorted by inclusion such that a cluster node comes before
	// its contained nodes (and clusters)
	Node *m_firstNode;  // first node in the list of all nodes
	Node *m_lastNode;   // last node in the list of all nodes

	const Options &m_options;  // reference to gml2pic options
};


#endif

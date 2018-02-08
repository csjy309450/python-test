/******************************************************************************
 * File:   OGMLProcessor.cpp
 * Author: Carsten Gutwenger
 *
 * Implements class OGMLProcessor, which is responsible for loading and
 * rendering graphs from GML files.
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


#include <QFile>
#include <ogdf/basic/basic.h>
#include "OGMLProcessor.h"


OGMLProcessor::OGMLProcessor(const Options &opt) : m_graph(opt)
{
	m_table["width"        ] = widthKey;
	m_table["height"       ] = heightKey;
	m_table["shape"        ] = shapeKey;
	m_table["color"        ] = colorKey;
	m_table["pattern"      ] = patternKey;
	m_table["patternColor" ] = patternColorKey;
	m_table["type"         ] = typeKey;
	m_table["size"         ] = sizeKey;
	m_table["textAlign"    ] = horizontalAlignKey;
	m_table["verticalAlign"] = verticalAlignKey;
	m_table["decoration"   ] = decorationKey;
	m_table["transform"    ] = transformKey;
	m_table["family"       ] = familyKey;
	m_table["variant"      ] = variantKey;
	m_table["weight"       ] = weightKey;
	m_table["stretch"      ] = stretchKey;
	m_table["style"        ] = styleKey;

	// just for backward compatibility
	m_table["horizont-align"] = horizontalAlignKey;
	m_table["vertical-align"] = verticalAlignKey;
}


void OGMLProcessor::readNodeTag()
{
	OGMLGraph::Node *x = m_graph.createNode(m_xml.attributes().value("id").toString());

	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "node") {
			m_graph.setNodeIsCluster(x);
			readNodeTag();
		} else if(m_xml.name() == "label")
			readLabelTag(x);
		else
			m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readEdgeTag()
{
	OGMLGraph::Edge *x = m_graph.getEdge(m_xml.attributes().value("id").toString());

	bool isFirstSrc = true;
	bool isFirstTgt = true;

	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "label")
			readLabelTag();

		else if(m_xml.name() == "source") {
			if(isFirstSrc) {
				x->m_src = m_graph.getNode(m_xml.attributes().value("idRef").toString());
				isFirstSrc = false;
			} else {
				cout << "Warning: several source nodes of an edge defined!" << endl;
			}
			while(m_xml.readNextStartElement()) {
				if(m_xml.name() == "label")
					readLabelTag();
				else
					m_xml.skipCurrentElement();
			}

		} else if(m_xml.name() == "target") {
			if(isFirstTgt) {
				x->m_tgt = m_graph.getNode(m_xml.attributes().value("idRef").toString());
				isFirstTgt = false;
			} else {
				cout << "Warning: several target nodes of an edge defined!" << endl;
			}
			while(m_xml.readNextStartElement()) {
				if(m_xml.name() == "label")
					readLabelTag();
				else
					m_xml.skipCurrentElement();
			}
		} else
			m_xml.skipCurrentElement();
	}

	if(isFirstSrc || isFirstTgt)
		cout << "Warning: Edge without source and target node found!" << endl;
}


void OGMLProcessor::readLabelTag(OGMLGraph::Node *parentNode)
{
	OGMLGraph::Label *x = m_graph.getLabel(m_xml.attributes().value("id").toString());
	x->m_parentNode = parentNode;

	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "content") {
			x->m_content = m_xml.readElementText();
		} else
			m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readStructureTag()
{
	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "node")
			readNodeTag();
		else if(m_xml.name() == "edge")
			readEdgeTag();
		else if(m_xml.name() == "label")
			readLabelTag();
		else
			m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readLayoutTag()
{
	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "styles")
			readStylesTag();
		else if(m_xml.name() == "styleTemplates")
			m_xml.skipCurrentElement();  // not yet supported
		else
			m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readStylesTag()
{
	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "nodeStyle")
			readNodeStyleTag();
		else if(m_xml.name() == "edgeStyle")
			readEdgeStyleTag();
		else if(m_xml.name() == "labelStyle")
			readLabelStyleTag();
		else if(m_xml.name() == "graphStyles")
			m_xml.skipCurrentElement(); // not yet supported
		else
			m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readNodeStyleTag()
{
	OGMLGraph::Node *x = m_graph.getNode(m_xml.attributes().value("idRef").toString());

	while(m_xml.readNextStartElement())
	{
		if(m_xml.name() == "location")
			x->m_location = QPointF(m_xml.attributes().value("x").toString().toDouble(), m_xml.attributes().value("y").toString().toDouble());

		else if(m_xml.name() == "shape")
		{
			qreal w = 0;
			qreal h = 0;
			Shape shape = shRect;

			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				QStringRef name = a.name();
				switch(m_table[a.name().toString()])
				{
				case widthKey:
					w = a.value().toString().toDouble();
					break;
				case heightKey:
					h = a.value().toString().toDouble();
					break;
				case typeKey:
					shape = m_graph.toShape(a.value().toString());
					break;
				}
			}
			x->m_nodeShape = NodeShapeFactory::create(shape,w,h);

		} else if(m_xml.name() == "fill") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case colorKey:
					x->m_fillColor.setNamedColor(a.value().toString());
					break;
				case patternKey:
					x->m_pattern = m_graph.toPattern(a.value().toString());
					break;
				case patternColorKey:
					x->m_fillPatternColor.setNamedColor(a.value().toString());
					break;
				}
			}

		} else if(m_xml.name() == "line") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case typeKey:
					x->m_lineType = m_graph.toLineType(a.value().toString());
					break;
				case colorKey:
					x->m_lineColor.setNamedColor(a.value().toString());
					break;
				case widthKey:
					x->m_lineWidth = a.value().toString().toDouble();
					break;
				}
			}
		}

		m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readEdgeStyleTag()
{
	OGMLGraph::Edge *x = m_graph.getEdge(m_xml.attributes().value("idRef").toString());

	while(m_xml.readNextStartElement())
	{
		if(m_xml.name() == "line") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case typeKey:
					x->m_lineType = m_graph.toLineType(a.value().toString());
					break;
				case colorKey:
					x->m_lineColor.setNamedColor(a.value().toString());
					break;
				case widthKey:
					x->m_lineWidth = a.value().toString().toDouble();
					break;
				}
			}

		} else if(m_xml.name() == "point") {
			x->m_points.pushBack(QPointF(m_xml.attributes().value("x").toString().toDouble(), m_xml.attributes().value("y").toString().toDouble()));

		} else if (m_xml.name() == "sourceStyle") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case typeKey:
					x->m_srcType = m_graph.toEndpointType(a.value().toString());
					break;
				case colorKey:
					x->m_srcColor.setNamedColor(a.value().toString());
					break;
				case sizeKey:
					x->m_srcSize = a.value().toString().toDouble();
					break;
				}
			}

		} else if (m_xml.name() == "targetStyle") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case typeKey:
					x->m_tgtType = m_graph.toEndpointType(a.value().toString());
					break;
				case colorKey:
					x->m_tgtColor.setNamedColor(a.value().toString());
					break;
				case sizeKey:
					x->m_tgtSize = a.value().toString().toDouble();
					break;
				}
			}
		}

		m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readLabelStyleTag()
{
	OGMLGraph::Label *x = m_graph.getLabel(m_xml.attributes().value("idRef").toString());

	while(m_xml.readNextStartElement())
	{
		if(m_xml.name() == "location")
			x->m_location = QPointF(m_xml.attributes().value("x").toString().toDouble(), m_xml.attributes().value("y").toString().toDouble());

		else if(m_xml.name() == "boundingBox") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case widthKey:
					x->m_width = a.value().toString().toDouble();
					break;
				case heightKey:
					x->m_height = a.value().toString().toDouble();
					break;
				}
			}
		} else if(m_xml.name() == "text") {
			int hAlign = 0;
			int vAlign = 0;

			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case horizontalAlignKey:
					hAlign = m_graph.toHAlign(a.value().toString());
					break;
				case verticalAlignKey:
					vAlign = m_graph.toVAlign(a.value().toString());
					break;
				case decorationKey:
					switch(m_graph.toDecorationType(a.value().toString()))
					{
					case OGMLGraph::dtUnderline:
						x->m_underline = 1; break;
					case OGMLGraph::dtOverline:
						x->m_overline = 1; break;
					case OGMLGraph::dtLineThrough:
						x->m_strikeOut = 1; break;
					}
					break;
				case transformKey:
					x->m_capitalization = m_graph.toCapitalizationType(a.value().toString());
					break;
				}
			}

			x->m_alignment = hAlign | vAlign;

		} else if(m_xml.name() == "font") {
			for(int i = 0; i < m_xml.attributes().size(); ++i) {
				QXmlStreamAttribute a = m_xml.attributes()[i];
				switch(m_table[a.name().toString()])
				{
				case familyKey:
					x->m_family = m_graph.hashString(a.value().toString());
					break;
				case variantKey:
					if(a.value() == "smallCaps")
						x->m_capitalization = QFont::SmallCaps;
					break;
				case weightKey:
					x->m_weight = m_graph.toFontWeight(a.value().toString());
					break;
				case stretchKey:
					x->m_stretch = m_graph.toFontStretch(a.value().toString());
					break;
				case styleKey:
					x->m_style = m_graph.toFontStyle(a.value().toString());
					break;
				case colorKey:
					x->m_color.setNamedColor(a.value().toString());
					break;
				case sizeKey:
					x->m_pointSize = a.value().toString().toDouble();
					break;
				}
			}
		}

		m_xml.skipCurrentElement();
	}
}


void OGMLProcessor::readGraphTag()
{
	while(m_xml.readNextStartElement()) {
		if(m_xml.name() == "structure") {
			readStructureTag();
		} else if(m_xml.name() == "layout") {
			readLayoutTag();
		} else {
			m_xml.skipCurrentElement();
		}
	}
}


bool OGMLProcessor::read(const QString &filename)
{
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}

	m_xml.setDevice(&file);

	if(m_xml.readNextStartElement())
	{
		if(m_xml.name() == "ogml") {
			while (m_xml.readNextStartElement()) {
				if(m_xml.name() == "graph")
					readGraphTag();
				else
					m_xml.skipCurrentElement();
			}
			if(m_xml.hasError()) {
				outputError();
				return false;
			}
			return true;

		} else {
			cout << "ogml tag not found!" << endl;
		}
	} else {
		outputError();
	}

	return false;
}


void OGMLProcessor::outputError()
{
	cout << "Error while parsing OGML file." << endl;
	switch(m_xml.error()) {
	case QXmlStreamReader::UnexpectedElementError:
		cout << "    Unexpected element: ";
		break;
	case QXmlStreamReader::CustomError:
		cout << "    Custom error: ";
		break;
	case QXmlStreamReader::NotWellFormedError:
		cout << "    Not well-formed XML: ";
		break;
	case QXmlStreamReader::PrematureEndOfDocumentError:
		cout << "    Premature end of document: ";
		break;
	default:
		cout << "   Error code " << m_xml.error() << ": ";
	}
	cout << m_xml.errorString() << endl;
	cout << "    at line " << m_xml.lineNumber() << ", column " << m_xml.columnNumber() << endl;
}

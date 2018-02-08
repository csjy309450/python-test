/******************************************************************************
 * File:   GMLProcessor.cpp
 * Author: Carsten Gutwenger
 *
 * Implements class GMLProcessor, which is responsible for loading and
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


#include "GMLProcessor.h"
#include <sstream>
#include <ogdf/fileformats/XmlParser.h>


bool GMLProcessor::read(const QString &filename)
{
	return m_cga.readClusterGML(filename.toStdString().c_str(), m_cg, m_graph);
}


QRectF GMLProcessor::boundingBox() const
{
	if(m_graph.empty()) return QRectF(0,0,0,0);

	node v = m_graph.firstNode();
	qreal xmin = m_cga.x(v), xmax = m_cga.x(v), ymin = m_cga.y(v), ymax = m_cga.y(v);

	forall_nodes(v,m_graph)
	{
		qreal w2 = 0.5 * (m_cga.width (v) + m_cga.lineWidthNode(v));
		qreal h2 = 0.5 * (m_cga.height(v) + m_cga.lineWidthNode(v));

		xmin = min(xmin, m_cga.x(v) - w2);
		xmax = max(xmax, m_cga.x(v) + w2);
        ymin = min(ymin, m_cga.y(v) - h2);
        ymax = max(ymax, m_cga.y(v) + h2);
	}

	edge e;
    forall_edges(e, m_graph)
	{
        const DPolyline &dpl = m_cga.bends(e);
		qreal pw2 = 0.5 * m_cga.edgeWidth(e);

        ListConstIterator<DPoint> it;
        for (it = dpl.begin(); it.valid(); ++it) {
			xmin = min(xmin, (*it).m_x - pw2);
			xmax = max(xmax, (*it).m_x + pw2);
			ymin = min(ymin, (*it).m_y - pw2);
			ymax = max(ymax, (*it).m_y + pw2);
        }
    }

	cluster c;
	forall_clusters(c,m_cg)
	{
		if(c == m_cg.rootCluster())
			continue;

		qreal pw2 = 0.5 * m_cga.clusterLineWidth(c);

		xmin = min(xmin, m_cga.clusterXPos(c) - pw2);
		xmax = max(xmax, m_cga.clusterXPos(c) + m_cga.clusterWidth(c) + pw2);
        ymin = min(ymin, m_cga.clusterYPos(c) - pw2);
        ymax = max(ymax, m_cga.clusterYPos(c) + m_cga.clusterHeight(c) + pw2);
	}

	return QRectF(xmin, ymin, xmax-xmin+0.5, ymax-ymin+0.5);
}


// convert hex digit to integer
inline int hexToInt(char c)
{
	return (isdigit(c)) ? int(c - '0') : int(c - 'a' + 10);
}


// converts a color from a string as given in GML to a QColor
QColor colorFromString(const QString &str)
{
	if(str.length() == 8)
	{
		int alpha = (hexToInt(str[0].toAscii()) << 4) + hexToInt(str[1].toAscii());
		int red   = (hexToInt(str[2].toAscii()) << 4) + hexToInt(str[3].toAscii());
		int green = (hexToInt(str[4].toAscii()) << 4) + hexToInt(str[5].toAscii());
		int blue  = (hexToInt(str[6].toAscii()) << 4) + hexToInt(str[7].toAscii());
		return QColor(red,green,blue,alpha);
	}
	return QColor();
}


// converts XML string to string (translates some encoded special characters)
QString fromXMLString(const char *xmlStr)
{
	QString str;
	str.reserve(strlen(xmlStr));

	while (*xmlStr != 0)
	{
		const char c = *xmlStr;

		if(c != '&') {
			str += c;
			++xmlStr;

		} else {
			++xmlStr;
			if(strncmp(xmlStr, "amp;", 4) == 0) {
				str += "&";
				xmlStr += 4;
			} else if(strncmp(xmlStr, "lt;", 3) == 0) {
				str += "<";
				xmlStr += 3;
			} else if(strncmp(xmlStr, "gt;", 3) == 0) {
				str += ">";
				xmlStr += 3;
			} else if(strncmp(xmlStr, "quot;", 5) == 0) {
				str += "\"";
				xmlStr += 5;
			} else if(strncmp(xmlStr, "apos;", 5) == 0) {
				str += "\'";
				xmlStr += 5;
			}
		}
	}

	return str;
}


// retrieve text and formatting info from string representation
void GMLProcessor::labelFromString(const char *attrStr, QColor &myTextColor, QFont &myFont, int &myAlign, QString &myLabel)
{
	//std::istrstream is(attrStr);
	std::istringstream is(attrStr);
	XmlParser xml(is);

	if (xml.error())
		return;

	const int pdLabel     = xml.getId("label");
	const int pdFont      = xml.getId("font");
	const int pdAlign     = xml.getId("align");
	const int pdTextColor = xml.getId("textColor");

	QColor textColor = myTextColor;
	QFont    font    = myFont;
	int      align   = myAlign;
	QString  text;

	XmlObject *root = xml.root();
	for(; root; root = root->m_pBrother)
	{
		int idRoot = xml.id(root);
		if(idRoot == pdLabel && root->m_valueType == xmlListBegin)
		{
			// label
			XmlObject *son = root->m_pFirstSon;
			for(; son; son = son->m_pBrother)
			{
				int id = xml.id(son);

				if(id == pdFont && son->m_valueType == xmlStringValue) {
					font.fromString(son->m_stringValue);

				} else if(id == pdAlign && son->m_valueType == xmlIntValue) {
					align = son->m_intValue;

				} else if(id == pdTextColor && son->m_valueType == xmlStringValue) {
					textColor = colorFromString(son->m_stringValue);

				} else if(id == pdLabel && son->m_valueType == xmlStringValue) {
					text = fromXMLString(son->m_stringValue);
				}
			} // for all sons of label
		}
	} // for all roots

	if(text.isNull()) {
		// support for old format
		myLabel = attrStr;

	} else {
		myFont     = font;
		myAlign    = align;
		myTextColor = textColor;
		myLabel    = text;
	}
}

/******************************************************************************
 * File:   OGMLGraph.cpp
 * Author: Carsten Gutwenger
 *
 * Implements class OGMLGraph, which represents a graph layout stored in the
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


#include "OGMLGraph.h"



OGMLGraph::OGMLGraph(const Options &opt) : m_options(opt)
{
	m_firstNode = m_lastNode = 0;

	// supported shape types
	m_stringTable.insert(EnumString(eShapeType, "rect"),             shRect);
	m_stringTable.insert(EnumString(eShapeType, "roundedRect"),      shRoundedRect);
	m_stringTable.insert(EnumString(eShapeType, "ellipse"),          shEllipse);
	m_stringTable.insert(EnumString(eShapeType, "triangle"),         shTriangle);
	m_stringTable.insert(EnumString(eShapeType, "pentagon"),         shPentagon);
	m_stringTable.insert(EnumString(eShapeType, "hexagon"),          shHexagon);
	m_stringTable.insert(EnumString(eShapeType, "octagon"),          shOctagon);
	m_stringTable.insert(EnumString(eShapeType, "rhomb"),            shRhomb);
	m_stringTable.insert(EnumString(eShapeType, "trapeze"),          shTrapeze);
	m_stringTable.insert(EnumString(eShapeType, "parallelogram"),    shParallelogram);
	m_stringTable.insert(EnumString(eShapeType, "invTriangle"),      shInvTriangle);
	m_stringTable.insert(EnumString(eShapeType, "invTrapeze"),       shInvTrapeze);
	m_stringTable.insert(EnumString(eShapeType, "invParallelogram"), shInvParallelogram);

	// supported pattern types
	m_stringTable.insert(EnumString(ePatternType, "noFill"),    Qt::NoBrush);
	m_stringTable.insert(EnumString(ePatternType, "solid"),     Qt::SolidPattern);
	m_stringTable.insert(EnumString(ePatternType, "dense1"),    Qt::Dense1Pattern);
	m_stringTable.insert(EnumString(ePatternType, "dense2"),    Qt::Dense2Pattern);
	m_stringTable.insert(EnumString(ePatternType, "dense3"),    Qt::Dense3Pattern);
	m_stringTable.insert(EnumString(ePatternType, "dense4"),    Qt::Dense4Pattern);
	m_stringTable.insert(EnumString(ePatternType, "dense5"),    Qt::Dense5Pattern);
	m_stringTable.insert(EnumString(ePatternType, "dense6"),    Qt::Dense6Pattern);
	m_stringTable.insert(EnumString(ePatternType, "dense7"),    Qt::Dense7Pattern);
	m_stringTable.insert(EnumString(ePatternType, "hor"),       Qt::HorPattern);
	m_stringTable.insert(EnumString(ePatternType, "ver"),       Qt::VerPattern);
	m_stringTable.insert(EnumString(ePatternType, "cross"),     Qt::CrossPattern);
	m_stringTable.insert(EnumString(ePatternType, "bDiag"),     Qt::BDiagPattern);
	m_stringTable.insert(EnumString(ePatternType, "fDiag"),     Qt::FDiagPattern);
	m_stringTable.insert(EnumString(ePatternType, "diagCross"), Qt::DiagCrossPattern);

	// (just for backward compatibility)
	m_stringTable.insert(EnumString(ePatternType, "none"),             Qt::NoBrush);
	m_stringTable.insert(EnumString(ePatternType, "bpNone"),           Qt::NoBrush);
	m_stringTable.insert(EnumString(ePatternType, "bpSolid"),          Qt::SolidPattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense1"),         Qt::Dense1Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense2"),         Qt::Dense2Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense3"),         Qt::Dense3Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense4"),         Qt::Dense4Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense5"),         Qt::Dense5Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense6"),         Qt::Dense6Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpDense7"),         Qt::Dense7Pattern);
	m_stringTable.insert(EnumString(ePatternType, "bpHorizontal"),     Qt::HorPattern);
	m_stringTable.insert(EnumString(ePatternType, "bpVertical"),       Qt::VerPattern);
	m_stringTable.insert(EnumString(ePatternType, "bpCross"),          Qt::CrossPattern);
	m_stringTable.insert(EnumString(ePatternType, "BackwardDiagonal"), Qt::BDiagPattern);
	m_stringTable.insert(EnumString(ePatternType, "ForwardDiagonal"),  Qt::FDiagPattern);
	m_stringTable.insert(EnumString(ePatternType, "DiagonalCross"),    Qt::DiagCrossPattern);

	// supported line types
	m_stringTable.insert(EnumString(eLineType, "none"),       Qt::NoPen);
	m_stringTable.insert(EnumString(eLineType, "solid"),      Qt::SolidLine);
	m_stringTable.insert(EnumString(eLineType, "dash"),       Qt::DashLine);
	m_stringTable.insert(EnumString(eLineType, "dot"),        Qt::DotLine);
	m_stringTable.insert(EnumString(eLineType, "dashDot"),    Qt::DashDotLine);
	m_stringTable.insert(EnumString(eLineType, "dashDotDot"), Qt::DashDotDotLine);

	// (just for backward compatibility)
	m_stringTable.insert(EnumString(eLineType, "esNoPen"),      Qt::NoPen);
	m_stringTable.insert(EnumString(eLineType, "esSolid"),      Qt::SolidLine);
	m_stringTable.insert(EnumString(eLineType, "esDash"),       Qt::DashLine);
	m_stringTable.insert(EnumString(eLineType, "esDot"),        Qt::DotLine);
	m_stringTable.insert(EnumString(eLineType, "esDashdot"),    Qt::DashDotLine);
	m_stringTable.insert(EnumString(eLineType, "esDashdotdot"), Qt::DashDotDotLine);

	// supported endpoint types
	m_stringTable.insert(EnumString(eEndpointType, "none"),      stNone);
	m_stringTable.insert(EnumString(eEndpointType, "arrow"),     stArrow);
	m_stringTable.insert(EnumString(eEndpointType, "triangle"),  stTriangle);
	m_stringTable.insert(EnumString(eEndpointType, "oTriangle"), stOTriangle);
	m_stringTable.insert(EnumString(eEndpointType, "circle"),    stCircle);
	m_stringTable.insert(EnumString(eEndpointType, "oCircle"),   stOCircle);
	m_stringTable.insert(EnumString(eEndpointType, "box"),       stBox);
	m_stringTable.insert(EnumString(eEndpointType, "oBox"),      stOBox);
	m_stringTable.insert(EnumString(eEndpointType, "rhomb"),     stRhomb);
	m_stringTable.insert(EnumString(eEndpointType, "oRhomb"),    stORhomb);
	m_stringTable.insert(EnumString(eEndpointType, "vee"),       stVee);
	m_stringTable.insert(EnumString(eEndpointType, "tee"),       stTee);

	// supported text alignment types
	m_stringTable.insert(EnumString(eHAlignType, "left"),    Qt::AlignLeft);
	m_stringTable.insert(EnumString(eHAlignType, "right"),   Qt::AlignRight);
	m_stringTable.insert(EnumString(eHAlignType, "center"),  Qt::AlignHCenter);
	m_stringTable.insert(EnumString(eHAlignType, "justify"), Qt::AlignJustify);

	// supported vertical alignment types
	m_stringTable.insert(EnumString(eVAlignType, "top"),     Qt::AlignTop);
	m_stringTable.insert(EnumString(eVAlignType, "middle"),  Qt::AlignVCenter);
	m_stringTable.insert(EnumString(eVAlignType, "bottom"),  Qt::AlignBottom);

	// supported decoration types
	m_stringTable.insert(EnumString(eDecorationType, "none"),        dtNone);
	m_stringTable.insert(EnumString(eDecorationType, "underline"),   dtUnderline);
	m_stringTable.insert(EnumString(eDecorationType, "overline"),    dtOverline);
	m_stringTable.insert(EnumString(eDecorationType, "lineThrough"), dtLineThrough);

	// supported capitalization types
	m_stringTable.insert(EnumString(eCapitalizationType, "capitalize"), QFont::Capitalize);
	m_stringTable.insert(EnumString(eCapitalizationType, "uppercase"),  QFont::AllUppercase);
	m_stringTable.insert(EnumString(eCapitalizationType, "lowercase"),  QFont::AllLowercase);
	m_stringTable.insert(EnumString(eCapitalizationType, "none"),       QFont::MixedCase);

	// supported font stretch types
	m_stringTable.insert(EnumString(eFontStretchType, "ultraCondensed"), QFont::UltraCondensed);
	m_stringTable.insert(EnumString(eFontStretchType, "extraCondensed"), QFont::ExtraCondensed);
	m_stringTable.insert(EnumString(eFontStretchType, "condensed"),      QFont::Condensed);
	m_stringTable.insert(EnumString(eFontStretchType, "semiCondensed"),  QFont::SemiCondensed);
	m_stringTable.insert(EnumString(eFontStretchType, "regular"),        QFont::Unstretched);
	m_stringTable.insert(EnumString(eFontStretchType, "semiExpanded"),   QFont::SemiExpanded);
	m_stringTable.insert(EnumString(eFontStretchType, "expanded"),       QFont::Expanded);
	m_stringTable.insert(EnumString(eFontStretchType, "extraExpanded"),  QFont::ExtraExpanded);
	m_stringTable.insert(EnumString(eFontStretchType, "ultraExpanded"),  QFont::UltraExpanded);

	// (just for backward compatibility)
	m_stringTable.insert(EnumString(eFontStretchType, "normal"),          QFont::Unstretched);
	m_stringTable.insert(EnumString(eFontStretchType, "wider"),           QFont::Expanded);
	m_stringTable.insert(EnumString(eFontStretchType, "narrower"),        QFont::Condensed);

	// supported font weight types
	m_stringTable.insert(EnumString(eFontWeightType, "light"),    QFont::Light);
	m_stringTable.insert(EnumString(eFontWeightType, "normal"),   QFont::Normal);
	m_stringTable.insert(EnumString(eFontWeightType, "demiBold"), QFont::DemiBold);
	m_stringTable.insert(EnumString(eFontWeightType, "bold"),     QFont::Bold);
	m_stringTable.insert(EnumString(eFontWeightType, "black"),    QFont::Black);
	m_stringTable.insert(EnumString(eFontWeightType, "100"),      10);
	m_stringTable.insert(EnumString(eFontWeightType, "200"),      QFont::Light);
	m_stringTable.insert(EnumString(eFontWeightType, "300"),      35);
	m_stringTable.insert(EnumString(eFontWeightType, "400"),      QFont::Normal);
	m_stringTable.insert(EnumString(eFontWeightType, "500"),      QFont::Normal);
	m_stringTable.insert(EnumString(eFontWeightType, "600"),      QFont::DemiBold);
	m_stringTable.insert(EnumString(eFontWeightType, "700"),      QFont::Bold);
	m_stringTable.insert(EnumString(eFontWeightType, "800"),      QFont::Black);
	m_stringTable.insert(EnumString(eFontWeightType, "900"),      95);

	// (just for backward compatibility)
	m_stringTable.insert(EnumString(eFontWeightType, "bolder"),  QFont::Black);
	m_stringTable.insert(EnumString(eFontWeightType, "lighter"), QFont::DemiBold);

	// supported font style types
	m_stringTable.insert(EnumString(eFontStyleType, "normal"),  QFont::StyleNormal);
	m_stringTable.insert(EnumString(eFontStyleType, "italic"),  QFont::StyleItalic);
	m_stringTable.insert(EnumString(eFontStyleType, "oblique"), QFont::StyleOblique);
}


OGMLGraph::~OGMLGraph()
{
	QHashIterator<QString,Pointer<Node> > itN(m_nodeTable);
	while(itN.hasNext()) {
		itN.next();
		delete itN.value().p;
	}

	QHashIterator<QString,Pointer<Edge> > itE(m_edgeTable);
	while(itE.hasNext()) {
		itE.next();
		delete itE.value().p;
	}

	QHashIterator<QString,Pointer<Label> > itL(m_labelTable);
	while(itL.hasNext()) {
		itL.next();
		delete itL.value().p;
	}
}



Shape OGMLGraph::toShape(const QString &str)
{
	IntType e = m_stringTable[EnumString(eShapeType,str)];
	return (e.m_int >= 0) ? (Shape)e.m_int : shRect;
}

EndpointType OGMLGraph::toEndpointType(const QString &str)
{
	IntType e = m_stringTable[EnumString(eEndpointType,str)];
	return (e.m_int >= 0) ? (EndpointType)e.m_int : stNone;
}

Qt::BrushStyle OGMLGraph::toPattern(const QString &str)
{
	IntType e = m_stringTable[EnumString(ePatternType,str)];
	return (e.m_int >= 0) ? (Qt::BrushStyle)e.m_int : Qt::SolidPattern;
}

Qt::PenStyle OGMLGraph::toLineType(const QString &str)
{
	IntType e = m_stringTable[EnumString(eLineType,str)];
	return (e.m_int >= 0) ? (Qt::PenStyle)e.m_int : Qt::SolidLine;
}

Qt::AlignmentFlag OGMLGraph::toHAlign(const QString &str)
{
	IntType e = m_stringTable[EnumString(eHAlignType,str)];
	return (e.m_int >= 0) ? (Qt::AlignmentFlag)e.m_int : Qt::AlignHCenter;
}

Qt::AlignmentFlag OGMLGraph::toVAlign(const QString &str)
{
	IntType e = m_stringTable[EnumString(eVAlignType,str)];
	return (e.m_int >= 0) ? (Qt::AlignmentFlag)e.m_int : Qt::AlignVCenter;
}

OGMLGraph::DecorationType OGMLGraph::toDecorationType(const QString &str)
{
	IntType e = m_stringTable[EnumString(eDecorationType,str)];
	return (e.m_int >= 0) ? (DecorationType)e.m_int : dtNone;
}

QFont::Capitalization OGMLGraph::toCapitalizationType(const QString &str)
{
	IntType e = m_stringTable[EnumString(eCapitalizationType,str)];
	return (e.m_int >= 0) ? (QFont::Capitalization)e.m_int : QFont::MixedCase;
}

uint OGMLGraph::toFontStretch(const QString &str)
{
	IntType e = m_stringTable[EnumString(eFontStretchType,str)];
	return (e.m_int >= 0) ? (uint)e.m_int : QFont::Unstretched;
}

uint OGMLGraph::toFontWeight(const QString &str)
{
	IntType e = m_stringTable[EnumString(eFontWeightType,str)];
	return (e.m_int >= 0) ? (uint)e.m_int : QFont::Normal;
}

uint OGMLGraph::toFontStyle(const QString &str)
{
	IntType e = m_stringTable[EnumString(eFontStyleType,str)];
	return (e.m_int >= 0) ? (uint)e.m_int : QFont::StyleNormal;
}


OGMLGraph::Node *OGMLGraph::createNode(const QString &id)
{
	Pointer<Node> &x = m_nodeTable[id];
	if(x.p == 0) {
		x.p = new Node();
		x.p->m_fillColor = m_options.defaultNodeColor;
	}

	if(m_firstNode != 0)
		m_lastNode->m_next = x.p;
	else
		m_firstNode = x.p;
	m_lastNode = x.p;

	++m_numNodes;

	return x.p;
}

OGMLGraph::Node *OGMLGraph::getNode(const QString &id)
{
	Pointer<Node> &x = m_nodeTable[id];
	if(x.p == 0) {
		x.p = new Node();
		x.p->m_fillColor = m_options.defaultNodeColor;
	}

	return x.p;
}


OGMLGraph::Edge *OGMLGraph::getEdge(const QString &id)
{
	Pointer<Edge> &x = m_edgeTable[id];
	if(x.p == 0) {
		x.p = new Edge();
		++m_numEdges;
	}

	return x.p;
}


OGMLGraph::Label *OGMLGraph::getLabel(const QString &id)
{
	Pointer<Label> &x = m_labelTable[id];
	if(x.p == 0) {
		x.p = new Label();
		x.p->m_color          = m_options.defaultLabelColor;
		x.p->m_family         = hashString(m_options.defaultLabelFont.family());
		x.p->m_pointSize      = m_options.defaultLabelFont.pointSizeF();
		x.p->m_style          = m_options.defaultLabelFont.style();
		x.p->m_weight         = m_options.defaultLabelFont.weight();
		x.p->m_stretch        = m_options.defaultLabelFont.stretch();
		x.p->m_capitalization = m_options.defaultLabelFont.capitalization();
		x.p->m_overline       = m_options.defaultLabelFont.overline();
		x.p->m_strikeOut      = m_options.defaultLabelFont.strikeOut();
		x.p->m_underline      = m_options.defaultLabelFont.underline();
	}

	return x.p;
}


QRectF OGMLGraph::boundingBox() const
{
	if(m_firstNode == 0) return QRectF(0,0,0,0);

	Node *v = m_firstNode;
	qreal xmin = v->m_location.x(), xmax = v->m_location.x(), ymin = v->m_location.y(), ymax = v->m_location.y();

	for(; v != 0; v = v->m_next)
	{
		if(v->m_nodeShape == 0)
			continue;

		qreal pw2 = 0.5 * max(1.0, v->m_lineWidth);

		xmin = min(xmin, v->m_location.x() - pw2);
		xmax = max(xmax, v->m_location.x() + v->m_nodeShape->width() + pw2);
		ymin = min(ymin, v->m_location.y() - pw2);
		ymax = max(ymax, v->m_location.y() + v->m_nodeShape->height() + pw2);
	}

	QHashIterator<QString,Pointer<Edge> > itE(m_edgeTable);
	while(itE.hasNext())
	{
		itE.next();
		Edge *e = itE.value().p;

		const List<QPointF> &dpl = e->m_points;
		qreal pw2 = 0.5 * max(1.0, e->m_lineWidth);

        ListConstIterator<QPointF> it;
        for (it = dpl.begin(); it.valid(); ++it) {
			xmin = min(xmin, (*it).x() - pw2);
			xmax = max(xmax, (*it).x() + pw2);
			ymin = min(ymin, (*it).y() - pw2);
			ymax = max(ymax, (*it).y() + pw2);
        }
	}

	// TO BE DONE: Handling of labels
	QHashIterator<QString,Pointer<Label> > itL(m_labelTable);
	while(itL.hasNext())
	{
		itL.next();
		Label *l = itL.value().p;

		QRectF bb = l->bb();
		if(bb.isEmpty())
			continue;

		xmin = min(xmin, bb.left());
		xmax = max(xmax, bb.right());
		ymin = min(ymin, bb.top());
		ymax = max(ymax, bb.bottom());
	}

	return QRectF(xmin, ymin, xmax-xmin/*+0.5*/, ymax-ymin/*+0.5*/);
}

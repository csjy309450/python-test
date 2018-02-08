/******************************************************************************
 * File:   drawing-OGML.cpp
 * Author: Carsten Gutwenger
 *
 * Implements the drawing function of OGMLProcessor, i.e., renders a layout
 * given by OGMLGraph on a paint device.
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
#include <limits>

#include "OGMLProcessor.h"
#include <QTextDocument>
#include <ogdf/basic/Math.h>


// normalizes edge polyline
// removes line segments completley covered by source or target rectangle
// adjust first and last point such that the polyline is attached at source
// and target rectangle
void OGMLProcessor::normalizeEdge(QPointF *points, int &head, int &tail,
	OGMLGraph::Node *src, OGMLGraph::Node *tgt)
{
	if(tail <= 0) return;

	int i = 1;
	while(tail-head+1 > 2 && src->contains(points[i])) {
		head++; i++;
	}
	points[head] = src->cutPoint(points[head], points[head+1]);

	i = tail-1;
	while(tail-head+1 > 2 && tgt->contains(points[i])) {
		tail--; --i;
	}

	points[tail] = tgt->cutPoint(points[tail], points[tail-1]);
	
}


void OGMLProcessor::draw(QPainter &painter)
{
	const Options &opt = m_graph.options();
	const int cMargin = 1;
	const int vMargin = 1;
	const qreal cMinEdgeWidth = EndpointShape::cMinEdgeWidth;

	// draw nodes and clusters
	for(OGMLGraph::Node *v = m_graph.firstNode(); v != 0; v = v->m_next)
	{
		if(v->m_nodeShape == 0)
			continue;

		qreal x = v->m_location.x();
		qreal y = v->m_location.y();

		QBrush brush(v->m_fillColor,v->m_pattern);
		painter.setBrush(brush);
		if(v->m_pattern == Qt::SolidPattern)
			painter.setBackgroundMode(Qt::OpaqueMode);
		else if(v->m_pattern != Qt::NoBrush && v->m_pattern != Qt::SolidPattern)
			painter.setBackground(v->m_fillPatternColor);

		QPen pen(v->m_lineColor, v->m_lineWidth, v->m_lineType,	Qt::SquareCap, Qt::RoundJoin);
		painter.setPen(pen);

		v->m_nodeShape->draw(painter,x,y);
	}

	QHashIterator<QString,OGMLGraph::Pointer<OGMLGraph::Edge> > itE = m_graph.beginEdges();
	while(itE.hasNext())
	{
		itE.next();
		OGMLGraph::Edge *e = itE.value().p;

		OGMLGraph::Node *src = e->m_src, *tgt = e->m_tgt;
		List<QPointF> dpl = e->m_points;

		qreal lineWidth = max(cMinEdgeWidth, e->m_lineWidth);
		QPen pen(e->m_lineColor, lineWidth, e->m_lineType, Qt::FlatCap, Qt::RoundJoin);
		painter.setPen(pen);

		if(dpl.empty()) {
			dpl.pushBack(src->center());
			dpl.pushBack(tgt->center());
		} else
			normalize(dpl);

		int head, tail;
		QPointF *points = newPointArray(dpl, head, tail);

		normalizeEdge(points, head, tail, src, tgt);
		if(tail - head <= 1 && points[head] == points[tail]) {  // edge has no length at all?
			delete [] points;
			continue;
		}

		QPointF posArrowSrc = points[head];
		QPointF posArrowTgt = points[tail];

		EndpointType srcType = stNone, tgtType = stArrow;

		switch(opt.arrowDisplay) {
		case Options::adFirst:
			srcType = stArrow;
			tgtType = stNone;
			break;
		case Options::adBoth:
			srcType = tgtType = stArrow;
			break;
		case Options::adNone:
			srcType = tgtType = stNone;
			break;
		case Options::adGML:
			srcType = e->m_srcType;
			tgtType = e->m_tgtType;
			break;
		default:
			;
		}

		EndpointShape *srcEPShape = EndpointShapeFactory::create(srcType, lineWidth, e->m_srcSize);
		EndpointShape *tgtEPShape = EndpointShapeFactory::create(tgtType, lineWidth, e->m_tgtSize);

		// adjust length of line segments
		qreal dl = (srcEPShape != 0) ? srcEPShape->length() : 0;
		qreal vx = points[head+1].x() - points[head].x();
		qreal vy = points[head+1].y() - points[head].y();
		qreal v_length = sqrt(vx*vx+vy*vy);
		dl = min(dl, 0.99*v_length);
		points[head].setX( points[head].x() + dl/v_length * vx);
		points[head].setY( points[head].y() + dl/v_length * vy);

		dl = (tgtEPShape != 0) ? tgtEPShape->length() : 0;
		vx = points[tail-1].x() - points[tail].x();
		vy = points[tail-1].y() - points[tail].y();
		v_length = sqrt(vx*vx+vy*vy);
		dl = min(dl, 0.99*v_length);
		points[tail].setX( points[tail].x() + dl/v_length * vx);
		points[tail].setY( points[tail].y() + dl/v_length * vy);

		// draw curve
		if(opt.curve == Options::ctLinear || opt.curvedLength <= std::numeric_limits<qreal>::epsilon())
			painter.drawPolyline(points+head, tail-head+1);

		else {
			QPainterPath path;

			switch(opt.curve) {
			case Options::ctQuadraticBezier:
				path = quadraticBezierPath(points+head,tail-head+1, opt.curvedLength);
				break;
			case Options::ctCubicBezier:
				path = cubicBezierPath(points+head,tail-head+1, opt.curvedLength);
				break;
			case Options::ctCircularArc:
				path = circularArcPath(points+head,tail-head+1, opt.curvedLength);
				break;

			OGDF_NODEFAULT
			}

			painter.setBrush(Qt::NoBrush);
			painter.drawPath(path);
		}

		// draw arrow heads
		if(srcEPShape != 0 || tgtEPShape != 0)
		{
			pen.setJoinStyle(Qt::MiterJoin);
			pen.setMiterLimit(4);
			pen.setStyle(Qt::SolidLine);

			if(srcEPShape != 0)
			{
				pen.setWidthF(srcEPShape->penWidth(lineWidth));
				pen.setCapStyle(srcEPShape->capStyle());
				painter.setPen(pen);

				double alpha = 180.0 / M_PI * atan2(points[head].x() - points[head+1].x(), points[head+1].y() - points[head].y());

				if(EndpointShape::isOpen(srcType))
					painter.setBrush(e->m_srcColor);
				else
					painter.setBrush(e->m_lineColor);
				painter.save();
				painter.translate(posArrowSrc);
				painter.rotate(alpha);
				srcEPShape->draw(painter);
				painter.restore();
				delete srcEPShape;
			}
			if(tgtEPShape != 0)
			{
				pen.setWidthF(tgtEPShape->penWidth(lineWidth));
				pen.setCapStyle(tgtEPShape->capStyle());
				painter.setPen(pen);

				double alpha = 180.0 / M_PI * atan2(points[tail].x() - points[tail-1].x(), points[tail-1].y() - points[tail].y());

				if(EndpointShape::isOpen(tgtType))
					painter.setBrush(e->m_tgtColor);
				else
					painter.setBrush(e->m_lineColor);
				painter.save();
				painter.translate(posArrowTgt);
				painter.rotate(alpha);
				tgtEPShape->draw(painter);
				painter.restore();
				delete tgtEPShape;
			}
		}
		delete [] points;
	}

	// draw labels
	QHashIterator<QString,OGMLGraph::Pointer<OGMLGraph::Label> > itL = m_graph.beginLabels();
	while(itL.hasNext())
	{
		itL.next();
		OGMLGraph::Label *l = itL.value().p;

		QRectF bb = l->bb();

		OGMLGraph::Node *v = l->m_parentNode;
		bool isCluster = (v != 0 && v->m_isCluster);

		// region for label defined by corresponding node?
		if(bb.isEmpty() && v != 0 && v->m_nodeShape != 0) {
			qreal dm = (isCluster) ? cMargin : vMargin;
			bb = v->bbLabel(dm);
		}

		if(bb.isEmpty())
			continue; // no region for label? cannot draw this one

		// simple check for html label
		bool isHtml = l->m_content.startsWith("<html>");

		if(isHtml) {
			QTextDocument doc;
			doc.setDefaultFont(opt.defaultLabelFont);
			doc.setDocumentMargin(0);
			doc.setHtml(l->m_content);
			doc.setTextWidth(bb.width());

			// vertical alignment
			qreal delta = max( 0.0, bb.height() - doc.size().height() );
			qreal margin_y;
			if(l->m_alignment & Qt::AlignTop)
				margin_y = 0;
			else if(l->m_alignment & Qt::AlignBottom)
				margin_y = delta;
			else
				margin_y = 0.5*delta;

			painter.setBackgroundMode(Qt::TransparentMode);
			painter.setFont(opt.defaultLabelFont);
			painter.setPen(Qt::black);

			painter.translate(bb.left(),bb.top()+margin_y);
			doc.drawContents(&painter, QRectF(0,0,bb.width(),bb.height()));
			painter.translate(-bb.left(),-bb.top()-margin_y);

		} else {
			QFont font;
			font.setFamily(l->m_family.key().m_string);
			font.setPointSizeF(l->m_pointSize);
			font.setStretch(l->m_stretch);
			font.setWeight(l->m_weight);
			font.setCapitalization((QFont::Capitalization)l->m_capitalization);
			font.setStyle((QFont::Style)l->m_style);
			font.setUnderline(l->m_underline);
			font.setOverline(l->m_overline);
			font.setStrikeOut(l->m_strikeOut);
			font.setStyleStrategy(QFont::PreferAntialias);

			QColor textColor = l->m_color;
			int align = l->m_alignment;
			if(align == 0)
				align = (isCluster) ? (Qt::AlignLeft | Qt::AlignTop) : Qt::AlignCenter;

			painter.setBackgroundMode(Qt::TransparentMode);
			painter.setFont(font);
			painter.setPen(textColor);

			painter.drawText(bb, align, l->m_content);
		}
	}
}

/******************************************************************************
 * File:   drawing-GML.cpp
 * Author: Carsten Gutwenger
 *
 * Implements the drawing function of GMLProcessor, i.e., renders a layout
 * given by ClusterGraphAttributes on a paint device.
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

#include "GMLProcessor.h"
#include <QTextDocument>
#include <ogdf/basic/Queue.h>
#include "EndpointShape.h"



void GMLProcessor::normalizeEdge(const NodeArray<NodeShape*> &vShape, 
	QPointF *points, int &head, int &tail, node src, node tgt)
{
	NodeShape *srcShape = vShape[src];
	qreal src_x = m_cga.x(src)-0.5*m_cga.width(src);
	qreal src_y = m_cga.y(src)-0.5*m_cga.height(src);

	if(tail <= 0) return;

	int i = 1;
	while(tail-head+1 > 2 && srcShape->contains(src_x,src_y,m_cga.lineWidthNode(src),points[i])) {
		head++; i++;
	}
	points[head] = srcShape->cutPoint(src_x, src_y, m_cga.lineWidthNode(src), points[head], points[head+1]);

	NodeShape *tgtShape = vShape[tgt];
	qreal tgt_x = m_cga.x(tgt)-0.5*m_cga.width(tgt);
	qreal tgt_y = m_cga.y(tgt)-0.5*m_cga.height(tgt);

	i = tail-1;
	while(tail-head+1 > 2 && tgtShape->contains(tgt_x,tgt_y,m_cga.lineWidthNode(tgt),points[i])) {
		tail--; --i;
	}

	points[tail] = tgtShape->cutPoint(tgt_x, tgt_y, m_cga.lineWidthNode(tgt), points[tail], points[tail-1]);
	
}


// compute the list of clusters in level order, i.e., clusters with increasing depth
void GMLProcessor::getLevelOrderClusters(const ClusterGraph &cg, SListPure<cluster> &clusters)
{
	Queue<cluster> Q;
	Q.append(cg.rootCluster());

	while(!Q.empty()) {
		cluster c = Q.pop();
		clusters.pushBack(c);

		ListConstIterator<cluster> it;
		for(it = c->cBegin(); it.valid(); ++it)
			Q.append(*it);
	}
}



// general drawing function
// draws given laoyuted graph onto a paint device using painter
void GMLProcessor::draw(QPainter &painter)
{
	const Options &opt = m_options;
	const ClusterGraphAttributes &cga = m_cga;
	const qreal cMinEdgeWidth = EndpointShape::cMinEdgeWidth;

	HashArray<String,Template> H(RectangleNodeSimple);
	H["oreas:std:rect simple"] = RectangleNodeSimple;
	H["oreas:std:rect"]        = RectangleNodeRT;
	H["oreas:std:ellipse"]     = EllipseNodeRT;
	H["oreas:std:hexagon"]     = HexagonNodeRT;
	H["oreas:std:rhombus"]     = RhombusNodeRT;
	H["oreas:std:trapeze"]     = TrapezeNodeRT;
	H["oreas:std:UML class"]   = UMLClassNode;      // not yet supported

	// same with "ogdf:" instead of "oreas"
	H["ogdf:std:rect simple"] = RectangleNodeSimple;
	H["ogdf:std:rect"]        = RectangleNodeRT;
	H["ogdf:std:ellipse"]     = EllipseNodeRT;
	H["ogdf:std:hexagon"]     = HexagonNodeRT;
	H["ogdf:std:rhombus"]     = RhombusNodeRT;
	H["ogdf:std:trapeze"]     = TrapezeNodeRT;
	H["ogdf:std:UML class"]   = UMLClassNode;      // not yet supported

	QPointF hp[6];

	const ClusterGraph &cg = cga.constClusterGraph();
	const Graph &graph = cg.getGraph();

	const int cMargin = 1;
	const int vMargin = 1;

	// create node shapes
	NodeArray<NodeShape*> vShape(graph);

	node v;
	forall_nodes(v,graph)
	{
		Shape shape = shRect;

		Template tpl = H[cga.templateNode(v)];
		switch(tpl) {
		case EllipseNodeRT:
			shape = shEllipse; break;
		case HexagonNodeRT:
			shape = shHexagon; break;
		case RhombusNodeRT:
			shape = shRhomb; break;
		case TrapezeNodeRT:
			shape = shTrapeze; break;
		}
		vShape[v] = NodeShapeFactory::create(shape,cga.width(v),cga.height(v));
	}

	// draw clusters
	// we want to draw clusters with smaller depth first, so that included clusters are drawn
	// later and thus in front of their parent clusters
	SListPure<cluster> clusters;
	getLevelOrderClusters(cg,clusters);

	SListConstIterator<cluster> itC;
	for(itC = clusters.begin(); itC.valid(); ++itC)
	{
		cluster c = *itC;
		if(c == cg.rootCluster())
			continue;

		double x = cga.clusterXPos(c);
		double y = cga.clusterYPos(c);
		double w = cga.clusterWidth(c);
		double h = cga.clusterHeight(c);

		QBrush brush(QColor(cga.clusterFillColor(c).cstr()),(Qt::BrushStyle)(cga.clusterFillPattern(c)));
		painter.setBrush(brush);

		QPen pen(QColor(cga.clusterColor(c).cstr()),
			cga.clusterLineWidth(c),
			(Qt::PenStyle)(cga.clusterLineStyle(c)),
			Qt::SquareCap, Qt::RoundJoin);

		painter.setPen(pen);
		painter.drawRect(QRectF(x,y,w,h));

		if(!opt.noClusterLabels && cga.clusterLabel(c).length() > 0)
		{
			QFont font = opt.defaultLabelFont;
			QColor textColor = opt.defaultLabelColor;
			int align = Qt::AlignLeft | Qt::AlignTop;
			QString label;

			labelFromString(cga.clusterLabel(c).cstr(), textColor, font, align, label);
			font.setStyleStrategy(QFont::PreferAntialias);

			painter.setBackgroundMode(Qt::TransparentMode);
			painter.setFont(font);
			painter.setPen(textColor);

			painter.drawText(x+cMargin, y+cMargin, w, h, align, label);
		}
	}
	
	// draw nodes
	forall_nodes(v,graph)
	{
		double x = cga.x(v);
		double y = cga.y(v);
		double w = cga.width(v);
		double h = cga.height(v);

		x = x-w/2;
		y = y-h/2;

		QColor nodeColor = (cga.colorNode(v).length() == 0) ? opt.defaultNodeColor : QColor(cga.colorNode(v).cstr());
		QBrush brush(nodeColor,(Qt::BrushStyle)(cga.nodePattern(v)));
		painter.setBrush(brush);
		QPen pen(QColor(cga.nodeLine(v).cstr()), cga.lineWidthNode(v), (Qt::PenStyle)(cga.styleNode(v)), Qt::SquareCap, Qt::RoundJoin);
		painter.setPen(pen);

		vShape[v]->draw(painter,x,y);

		qreal dm = vMargin;
		QRectF bb = vShape[v]->bbLabel(x,y,dm);

		switch(H[cga.templateNode(v)]) {
		case RectangleNodeRT:
		case EllipseNodeRT:
		case HexagonNodeRT:
		case RhombusNodeRT:
		case TrapezeNodeRT:
			if(!opt.noNodeLabels && cga.labelNode(v).length() > 0 && w > 0)
			{
				QTextDocument doc;
				doc.setDefaultFont(opt.defaultLabelFont);
				doc.setDocumentMargin(dm);
				doc.setHtml(cga.labelNode(v).cstr());
				doc.setTextWidth(bb.width());
				qreal h_text = doc.size().height();
				qreal margin_y = max(0.0, 0.5*(bb.height()-h_text));
				painter.setBackgroundMode(Qt::TransparentMode);
				painter.setFont(opt.defaultLabelFont);
				painter.setPen(Qt::black);

				painter.translate(bb.left(),bb.top()+margin_y);
				doc.drawContents(&painter, QRectF(0,0,bb.width(),bb.height()));
				painter.translate(-bb.left(),-bb.top()-margin_y);
			}
			break;

		default:
			if(!opt.noNodeLabels && cga.labelNode(v).length() > 0)
			{
				QFont font = opt.defaultLabelFont;
				QColor textColor = opt.defaultLabelColor;
				int align = Qt::AlignCenter;
				QString label;

				labelFromString(cga.labelNode(v).cstr(), textColor, font, align, label);
				font.setStyleStrategy(QFont::PreferAntialias);

				painter.setBackgroundMode(Qt::TransparentMode);
				painter.setFont(font);
				painter.setPen(textColor);

				painter.drawText(bb, align, label);
			}
		}
	}

	// draw edges
	QPointF auxHeadpoints[3];
	edge e;
	forall_edges(e,graph)
	{
		node src = e->source(), tgt = e->target();
		DPolyline dpl = cga.bends(e);

		QColor lineColor(cga.colorEdge(e).cstr());
		qreal lineWidth = max(cMinEdgeWidth, cga.edgeWidth(e));
		QPen pen(lineColor, lineWidth, (Qt::PenStyle)(cga.styleEdge(e)), Qt::FlatCap, Qt::RoundJoin);
		painter.setPen(pen);

		if(dpl.empty()) {
			dpl.pushBack(DPoint(cga.x(e->source()), cga.y(e->source())));
			dpl.pushBack(DPoint(cga.x(e->target()), cga.y(e->target())));
		} else
			dpl.normalize();

		int head, tail;
		QPointF *points = newPointArray(dpl, head, tail);

		normalizeEdge(vShape,points,head,tail,src,tgt);

		QPointF posArrowSrc = points[head];
		QPointF posArrowTgt = points[tail];

		EndpointType srcType = stNone;
		EndpointType tgtType = stArrow;

		GraphAttributes::EdgeArrow arrows = cga.arrowEdge(e);
		switch(opt.arrowDisplay) {
		case Options::adFirst:
			srcType = stArrow;
			tgtType = stNone;
			break;
		case Options::adGML:
			srcType = (arrows == GraphAttributes::first || arrows == GraphAttributes::both) ? stArrow : stNone;
			tgtType = (arrows == GraphAttributes::last || arrows == GraphAttributes::both) ? stArrow : stNone;
			break;
		case Options::adBoth:
			srcType = tgtType = stArrow;
			break;
		case Options::adNone:
			srcType = tgtType = stNone;
			break;
		}

		EndpointShape *srcEPShape = EndpointShapeFactory::create(srcType, lineWidth, 1.0);
		EndpointShape *tgtEPShape = EndpointShapeFactory::create(tgtType, lineWidth, 1.0);

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
					painter.setBrush(Qt::white);
				else
					painter.setBrush(lineColor);
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
					painter.setBrush(Qt::white);
				else
					painter.setBrush(lineColor);
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

	forall_nodes(v,graph)
		delete vShape[v];
}

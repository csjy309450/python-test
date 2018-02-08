/******************************************************************************
 * File:   gml2pic.h
 * Author: Carsten Gutwenger
 *
 * Declares some general enumeration types and functions, as well as the
 * Options class for storing all options that can be set by the user.
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


#ifndef _GML2PIC_H
#define _GML2PIC_H

#include <QColor>
#include <QPointF>
#include <QFont>
#include <ogdf/basic/basic.h>

using namespace ogdf;


#ifndef _MSC_VER
inline int _stricmp(const char *string1, const char *string2) {
	return strcasecmp(string1,string2);
}
#endif


/******************************************************************************
 *                               ENUMERATIONS
 ******************************************************************************/

// type for produced picture
enum PictureType {
	picPNG = 0, // PNG  bitmap format (portable network graphics)
	picJPEG,    // JPEG bitmap format (joint photographics expert group)
	picTIFF,    // TIFF bitmap format (tagged image file format)
	picSVG,     // SVG  vector format (scalable vector graphics)
	picPDF,     // PDF  vector format (portable document format)
	picEPS,     // EPS  vector format (encapsulated postscript)
	picSTOP
};


// type for input graph format
enum GraphFormatType {
	gfAUTO = 0, // automatic detection of format
	gfGML,      // GML format
	gfOGML,     // OGML format
	gfSTOP
};


// type for available node templates
enum Template {
	RectangleNodeSimple, // rectangle with simple node label
	RectangleNodeRT,     // rectangle with richt text node label
	EllipseNodeRT,       // ellipse   with richt text node label
	HexagonNodeRT,       // hexagone  with richt text node label
	RhombusNodeRT,       // rhombus   with richt text node label
	TrapezeNodeRT,       // trapeze   with richt text node label
	UMLClassNode         // UML class
};



/******************************************************************************
 *                              CLASS Options
 *
 * store the user options; sets the default values
 ******************************************************************************/

struct Options
{
	// display type for edge arrows heads
	enum ArrowDisplayType {
		adNone,   // do not display arrows at all
		adGML,    // use the information provided in the GML file
		adFirst,  // always show arrow head at source, but not at target
		adLast,   // always show arrow head at target, but not at source
		adBoth,   // always show arrow head at source and target
		adtSTOP
	} arrowDisplay;  // arrow head display format

	// curve type for smoothening polylines
	enum CurveType {
		ctLinear,           // just linear line segments, default
		ctQuadraticBezier,  // smoothen bends using quadratic bezier curves
		ctCubicBezier,      // smoothen bends using cubic bezier curves
		ctCircularArc,      // smoothen bends using circular arcs
		ctSTOP
	} curve; // curve format
	qreal curvedLength;

	// message level type (how much message output is produced)
	enum MessageLevelType {
		mlSilent,
		mlNormal,
		mlVerbose,
		mltSTOP
	} messageLevel;  // output message level (the higher the more output)

	qreal margin; // margin around drawing (in GML coordinates)

	PictureType pic; // output picture format
	GraphFormatType format; // input graph format

	int width;    // width for bitmap images
	int height;   // height for bitmap images
	double scale; // scale factor for bitmap images

	QColor background; // background color for bitmap images

	bool noNodeLabels;    // always omit node labels?
	bool noClusterLabels; // always omit cluster labels?

	int compressionLevel; // compression level for bitmap formats (0..100)
						  // high compression means smaller files

	// the following options are not yet adjustable via command line parameters

	QFont  defaultLabelFont;   // default font for labels
	QColor defaultLabelColor;  // default color for labels
	QColor defaultNodeColor;   // default color for nodes

	// constructor; sets default values
	Options() : defaultLabelColor(Qt::black), defaultNodeColor(Qt::white)
	{
		arrowDisplay = adGML;
		pic = picPNG;
		format = gfAUTO;

		curve = ctLinear;
		curvedLength = -1;

		margin = 0;

		messageLevel = mlNormal;

		width  = 0;
		height = 0;
		scale  = 0.0;

		noNodeLabels    = false;
		noClusterLabels = false;

		background = Qt::white;

		compressionLevel = -1;

		defaultLabelFont.setFamily("Arial");
		defaultLabelFont.setPointSizeF(12);
		defaultLabelFont.setStyleStrategy(QFont::PreferAntialias);
	}
};



/******************************************************************************
 *                             OUTPUT OPERATORS
 ******************************************************************************/

// overloaded output operator for Qt points
inline ostream &operator<<(ostream &os, const QPointF &p)
{
	os << "(" << p.x() << "," << p.y() << ")";
	return os;
}

// overloaded output operator for Qt strings
inline ostream &operator<<(ostream &os, const QString &str)
{
	os << str.toStdString();
	return os;
}


/******************************************************************************
 *                              VECTOR FUNCTIONS
 ******************************************************************************/

inline qreal euclideanLength(QPointF p)
{
	return sqrt( p.x()*p.x() + p.y()*p.y() );
}

inline void normalize(QPointF &p)
{
	p /= euclideanLength(p);
}

inline qreal dotProduct(QPointF p, QPointF q)
{
	return p.x() * q.x() + p.y() * q.y();
}


/******************************************************************************
 *                             PARSING FUNCTIONS
 ******************************************************************************/

inline bool parseNonNegReal(const char *option, const char *str, qreal &number)
{
	char *endptr;

	number = strtod(str, &endptr);
	if(*endptr != '\0') {
		cout << "argument of " << option << " is not a number: " << str << endl;
		return false;
	}
	if(number < 0) {
		cout << "argument of " << option << " must be non-negative!" << endl;
		return false;
	}

	return true;
}

inline bool parsePosReal(const char *option, const char *str, qreal &number)
{
	char *endptr;

	number = strtod(str, &endptr);
	if(*endptr != '\0') {
		cout << "argument of " << option << " is not a number: " << str << endl;
		return false;
	}
	if(number <= 0) {
		cout << "argument of " << option << " must be positive!" << endl;
		return false;
	}

	return true;
}

inline bool parseNonNegInteger(const char *option, const char *str, int &number)
{
	char *endptr;

	number = strtol(str, &endptr, 0);
	if(*endptr != '\0') {
		cout << "argument of " << option << " is not an integer number: " << str << endl;
		return false;
	}
	if(number < 0) {
		cout << "argument of " << option << " must be non-negative!" << endl;
		return false;
	}

	return true;
}

#endif

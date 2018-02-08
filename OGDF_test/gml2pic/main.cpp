/******************************************************************************
 * File:   main.cpp
 * Author: Carsten Gutwenger
 *
 * Implements gml2pic's main function.
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
#include "OGMLProcessor.h"

#include <QSvgGenerator>
#include <QPrinter>
#include <QApplication>
#include <QPixmap>

using namespace ogdf;


const char *version = "2.0.0";


/******************************************************************************
 *                         strings tags for enumerations
 ******************************************************************************/

// tags for picture formats
const char *picTag[] = {
	"png", "jpg", "tiff", "svg", "pdf", "eps"
};

// tags for graph formats
const char *formatTag[] = {
	"auto", "gml", "ogml"
};

// tags for arrows display formats
const char *adtTag[] = {
	"none", "gml", "first", "last", "both"
};

// tags for curve formats
const char *ctTag[] = {
	"linear", "quad", "cubic", "arc"
};

// tags for message levels
const char *mltTag[] = {
	"silent", "normal", "verbose"
};



/******************************************************************************
 *                             HELPER FUNCTIONS
 ******************************************************************************/

// replace file name extension
// (usually from ".gml" to extension of graphics format like ".png" etc.)
QString changeExtension(QString filename, QString extension)
{
	int i = filename.length() - 1;
	while(i >= 0 && filename[i] != '.' && filename[i] != '/' && filename[i] != '\\' && filename[i] != ':')
		--i;

	if(i >= 0 && filename[i] == '.')
		return filename.left(i+1) + extension;
	else
		return filename + '.' + extension;
}


void decomposeFilename(QString &filename, QString &dir, QString &pattern)
{
	int n = filename.length();
	for(int i = 0; i < n; ++i)
		if(filename[i] == '\\') filename[i] = '/';

	int i = n-1;
	while(i >= 0 && filename[i] != '/' && filename[i] != ':')
		--i;

	if(i >= 0) {
		dir = filename.left(i+1);
		pattern = filename.right(n-i-1);
	} else {
		dir = "./";
		pattern = filename;
	}
}


// guess the graph format from input file
GraphFormatType guessGraphFormat(const QString &file)
{
	// currently very simple, just look at the file extension
	if(file.endsWith(".ogml"))
		return gfOGML;

	// fall back / default is GML
	return gfGML;
}


// we define our own message handler to get rid off some Qt warnings, in particular
//  "QPixmap: Cannot create a QPixmap when no GUI is being used"
void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
	case QtDebugMsg:
		cerr << "Debug: " << msg << endl;
		break;
	case QtWarningMsg:
		//cerr << "Warning: " << msg << endl;
		break;
	case QtCriticalMsg:
		cerr << "Critical: " << msg << endl;
		break;
	case QtFatalMsg:
		cerr << "Fatal: " << msg << endl;
		abort();
	}
}


/******************************************************************************
 *                             MAIN FUNCTION
 ******************************************************************************/

int main(int argc, char *argv[])
{
	qInstallMsgHandler(myMessageOutput);

	QApplication app(argc, argv, false); // required for using QPrinter

	// too few arguments passed?
	if(argc <= 1) {
		cout << "Call with: gml2pic [ Options ] file.gml" << endl;
		cout << "Type \'gml2pic -help\' for help" << endl;
		return 1;
	}

	Options opt; // stores user options; set to default values

	QString filename;  // input GML file name
	QString outname;           // output graphics file name

	// process all arguments
	for(int i = 1; i < argc; ++i)
	{
		// picture type?
		if(strcmp(argv[i], "-pic") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			int j = 0;
			for(; j < (int)(picSTOP); ++j)
			{
				if(_stricmp(argv[i], picTag[j]) == 0) {
					opt.pic = (PictureType)(j);
					break;
				}
			}
			if(j == (int)(picSTOP)) {
				cout << "unrecognized picture type: " << argv[i] << endl;
				return 1;
			}

		// graph format?
		} else if(strcmp(argv[i], "-format") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			int j = 0;
			for(; j < (int)(gfSTOP); ++j)
			{
				if(_stricmp(argv[i], formatTag[j]) == 0) {
					opt.format = (GraphFormatType)(j);
					break;
				}
			}
			if(j == (int)(gfSTOP)) {
				cout << "unrecognized graph format: " << argv[i] << endl;
				return 1;
			}

		// message level?
		} else if(strcmp(argv[i], "-msg-level") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			int j = 0;
			for(; j < (int)(Options::mltSTOP); ++j)
			{
				if(_stricmp(argv[i], mltTag[j]) == 0) {
					opt.messageLevel = (Options::MessageLevelType)(j);
					break;
				}
			}
			if(j == (int)(Options::mltSTOP)) {
				cout << "unrecognized message level: " << argv[i] << endl;
				return 1;
			}

		// arrow display type?
		} else if(strcmp(argv[i], "-arrows") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			int j = 0;
			for(; j < (int)(Options::adtSTOP); ++j)
			{
				if(_stricmp(argv[i], adtTag[j]) == 0) {
					opt.arrowDisplay = (Options::ArrowDisplayType)(j);
					break;
				}
			}
			if(j == (int)(Options::adtSTOP)) {
				cout << "unrecognized arrow type: " << argv[i] << endl;
				return 1;
			}

		// curve type?
		} else if(strcmp(argv[i], "-curve") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			int j = 0;
			for(; j < (int)(Options::ctSTOP); ++j)
			{
				if(_stricmp(argv[i], ctTag[j]) == 0) {
					opt.curve = (Options::CurveType)(j);
					break;
				}
			}
			if(j == (int)(Options::ctSTOP)) {
				cout << "unrecognized curve type: " << argv[i] << endl;
				return 1;
			}

		// curved segment length?
		} else if(strcmp(argv[i], "-curvedLength") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			if(!parseNonNegReal(argv[i-1], argv[i], opt.curvedLength))
				return 1;

		// margin around drawing?
		} else if(strcmp(argv[i], "-margin") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			if(!parseNonNegReal(argv[i-1], argv[i], opt.margin))
				return 1;

		// output file name?
		} else if(strcmp(argv[i], "-o") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			outname = argv[i];

		// bitmap background color?
		} else if(strcmp(argv[i], "-bg") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			opt.background.setNamedColor(argv[i]);
			if(!opt.background.isValid()) {
				cout << "unrecognized background color: " << argv[i] << endl;
				return 1;
			}

		// bitmap width?
		} else if(strcmp(argv[i], "-width") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			if(!parseNonNegInteger(argv[i-1], argv[i], opt.width))
				return 1;

		// bitmap height?
		} else if(strcmp(argv[i], "-height") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			if(!parseNonNegInteger(argv[i-1], argv[i], opt.height))
				return 1;

		// scale factor for bitmap size?
		} else if(strcmp(argv[i], "-scale") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			if(!parseNonNegReal(argv[i-1], argv[i], opt.scale))
				return 1;

		// compression level?
		} else if(strcmp(argv[i], "-compression") == 0) {
			if(++i == argc) {
				cout << "missing argument for " << argv[i] << endl;
				return 1;
			}
			if(!parseNonNegInteger(argv[i-1], argv[i], opt.compressionLevel))
				return 1;
			if(opt.compressionLevel > 100) {
				cout << "compression level must be in [0..100], given value: " << opt.compressionLevel << endl;
				return 1;
			}

		// always omit node lables?
		} else if(strcmp(argv[i], "-no-node-labels") == 0) {
			opt.noNodeLabels = true;

		// always omit cluster lables?
		} else if(strcmp(argv[i], "-no-cluster-labels") == 0) {
			opt.noClusterLabels = true;

		// show version info?
		} else if(strcmp(argv[i], "-version") == 0) {
			cout << "gml2pic version " << version << " (Qt version " << qVersion() << ")." << endl;
			return 0;

		// show help?
		} else if(strcmp(argv[i], "-help") == 0) {
			cout << "Call with: gml2pic [ Options ] file.gml" << endl;
			cout << "\nwhere Options are:" << endl;
			cout << "  -format type" << endl;
			cout << "     specifies the file format of the input graph" << endl;
			cout << "     possible types are: auto, GML, OGML (default: auto)" << endl;
			cout << "  -pic type" << endl;
			cout << "     selects the type of image produced" << endl;
			cout << "     possible types are: PNG, JPG, TIFF, SVG, PDF, EPS (default: PNG)" << endl;
			cout << "  -o outfilename" << endl;
			cout << "     name of the generated file; if not specified, basename of input" << endl;
			cout << "     file with file extension of graphics format is used" << endl;
			cout << "  -msg-level level" << endl;
			cout << "     selects the message level, which determines how much informative output is produced" << endl;
			cout << "     possible levels are: silent, normal, verbose (default: normal)" << endl;

			cout << "  -no-node-labels" << endl;
			cout << "     do not draw node lables" << endl;
			cout << "  -no-cluster-labels" << endl;
			cout << "     do not draw cluster lables" << endl;

			cout << "  -margin size" << endl;
			cout << "     specifies an additional margin around the drawing (default: 0)" << endl;
			cout << "  -arrows type" << endl;
			cout << "     selects the display type for arrows" << endl;
			cout << "     possible types are: none, gml, first, last, both (default: gml)" << endl;
			cout << "  -curve type" << endl;
			cout << "     selects the curve type for smoothening bends" << endl;
			cout << "     possible types are: linear, quad, cubic, arc (default: linear)" << endl;
			cout << "  -curvedLength len" << endl;
			cout << "     specifies the maximal length of line segments that are curved;" << endl;
			cout << "     segments longer than 2*len will be drawn linear in the middle part" << endl;

			cout << "  -version" << endl;
			cout << "     shows version information and exits" << endl;
			cout << "  -help" << endl;
			cout << "     displays this help and exits" << endl;

			cout << "\n Options for bitmap formats" << endl;
			cout << "  -bg color" << endl;
			cout << "     selects the background color (default: white); possible colors are" << endl;
			cout << "     * #RGB      (each of R, G, B is a single hex digit)" << endl;
			cout << "     * #RRGGBB" << endl;
			cout << "     * #RRRGGGBBB" << endl;
			cout << "     * #RRRRGGGGBBBB" << endl;
			cout << "     * a name from the list of SVG color keyword names" << endl;
			cout << "     * transparent" << endl;
			cout << "  -width w" << endl;
			cout << "     the width of the bitmap in pixels" << endl;
			cout << "  -height h" << endl;
			cout << "     the height of the bitmap in pixels" << endl;
			cout << "  -scale f" << endl;
			cout << "     a scale factor for scaling the gml coordinates (default: 1.0)" << endl;
			cout << "     Note: you cannot define a scale factor AND width or height" << endl;
			cout << "     at the same time" << endl;
			cout << "  -compression level" << endl;
			cout << "     the compression level for bitmap graphics formats (default: -1)" << endl;
			cout << "       -1:     use default value for image format" << endl;
			cout << "       0..100: sets compression level (higher value means smaller files size)" << endl;

			return 0;

		// filename or error in arguments
		} else {
			if(i+1 < argc) {
				cout << "too many arguments without options:" << argv[i] << endl;
				return 1;
			}
			filename = argv[i];
		}
	}

	// no file name given?
	if(filename.isEmpty()) {
		cout << "missing filename" << endl;
		return 1;
	}

	// scale factor and width/height specified?
	if((opt.width > 0 || opt.height > 0) && opt.scale > 0.0) {
		cout << "defining both width (or height) and scale is not allowed" << endl;
		return 1;
	}

	// derive default values for curvedLength according to curve format?
	if(opt.curvedLength < 0) {
		switch(opt.curve) {
		case Options::ctQuadraticBezier:
			opt.curvedLength = 20; break;
		case Options::ctCubicBezier:
			opt.curvedLength = 40; break;
		case Options::ctCircularArc:
			opt.curvedLength = 25; break;
		default:
			; // nothing to do
		}
	}


	// determine directory and file/pattern name
	QString dir, pattern;
	decomposeFilename(filename, dir, pattern);

	// get list of all files fullfilling the pattern
	List<String> files;
	getFiles(dir.toStdString().c_str(), files, pattern.toStdString().c_str());

	// iterate over all files and produce graphics file
	ListConstIterator<String> itFile;
	for(itFile = files.begin(); itFile.valid(); ++itFile)
	{
		// full path name to file
		QString file = dir + (*itFile).cstr();

		// if no output file name is given, we derive it from the input file name
		QString outfile = (outname.isEmpty()) ? changeExtension(file,picTag[opt.pic]) : outname;

		// determine graph format
		GraphFormatType format = opt.format;
		if(format == gfAUTO)
			format = guessGraphFormat(file);

		// create graph processor
		GraphProcessor *processor;
		switch(format)
		{
		case gfOGML:
			processor = new OGMLProcessor(opt);
			break;
		default:
			processor = new GMLProcessor(opt);
		}

		// message output
		if(opt.messageLevel == Options::mlNormal)
			cout << file << endl;
		else if(opt.messageLevel == Options::mlVerbose) {
			cout << "Input format : " << QString(formatTag[format ]).toUpper() << endl;
			cout << "Output format: " << QString(picTag   [opt.pic]).toUpper() << endl;
			cout << "Reading " << file << "... ";
		}

		// read graph layout from file
		bool ok = processor->read(file);
		if(!ok) {
			cout << "Could not load graph: " << file << endl;
			return 1;
		}

		// message output
		if(opt.messageLevel == Options::mlVerbose) {
			cout << processor->numberOfNodes() << " nodes, " << processor->numberOfEdges() << " edges";
			if(processor->numberOfClusters() > 1) cout << ", " << processor->numberOfClusters() << " clusters";
			cout << " read." << endl;
			cout << "Write result to " << outfile << "." << endl;
		}

		// compute bounding box of layout
		QRectF bb = processor->boundingBox();
	
		qreal w    = bb.width()  + 2*opt.margin;
		qreal h    = bb.height() + 2*opt.margin;
		qreal left = bb.left()   - opt.margin;
		qreal top  = bb.top()    - opt.margin;

		if(opt.messageLevel == Options::mlVerbose) {
			cout << "Bounding box: top-left = " << bb.topLeft() << ", bottom-right = " << bb.bottomRight() << endl;
			cout << "View box:     left = " << left << ", top = " << top << ", width = " << w << ", height = " << h << endl;
		}

		// generate picture in desired graphics format
		switch(opt.pic) {
		case picJPEG:
		case picPNG:
		case picTIFF:
			{
				// bitmap graphics

				// "plus 1" for anit-aliasing
				if(opt.margin < 1) {
					w += (1 - opt.margin);
					h += (1 - opt.margin);
				}

				// determine size of bitmap
				int bmp_w = opt.width;
				int bmp_h = opt.height;

				if(opt.scale > 0) {
					bmp_w = ceil(opt.scale * w);
					bmp_h = ceil(opt.scale * h);
				} else if (bmp_w == 0 && bmp_h == 0) {
					bmp_w = ceil(w);
					bmp_h = ceil(h);
				} else if (bmp_h == 0) {
					qreal f = qreal(bmp_w) / w;
					bmp_h = ceil(f * h);
				} else {
					qreal f = qreal(bmp_h) / h;
					bmp_w = ceil(f * w);
				}

				if(opt.messageLevel == Options::mlVerbose)
					cout << "Bitmap-size: " << bmp_w << ", " << bmp_h << endl;

				QImage image(bmp_w, bmp_h, QImage::Format_ARGB32);
				image.fill(opt.background.rgba());

				QPainter painter(&image);
				painter.setRenderHint(QPainter::Antialiasing);
				painter.setRenderHint(QPainter::TextAntialiasing);

				painter.scale(double(bmp_w)/w, double(bmp_h)/h);
				painter.translate(-left,-top);

				processor->draw(painter);
				painter.end();

				switch(opt.pic) {
				case picJPEG:
					image.save(outfile, "JPG", (opt.compressionLevel == -1) ? 100 : 100-opt.compressionLevel);
					break;
				case picPNG:
					image.save(outfile, "PNG", (opt.compressionLevel == -1) ? 0 : 100-opt.compressionLevel);
					break;
				case picTIFF:
					image.save(outfile, "TIFF");
					break;
				default:
					; // nothing to do
				}
			}
		
			break;

		case picSVG:
			{
				// SVG vector graphics

				if(opt.messageLevel == Options::mlVerbose)
					cout << "SVG area: " << w << ", " << h << endl;

				QSvgGenerator svg;
				svg.setFileName(outfile);
				svg.setSize(QSize(w,h));
				svg.setViewBox(QRectF(0,0,w,h));

				QPainter painter(&svg);
				painter.translate(-left/*-0.5*/,-top/*-0.5*/);
				processor->draw(painter);
			}
			break;

		case picPDF:
		case picEPS:
			{
				// PDF/EPS vector graphics via printer driver

				QPrinter printer(QPrinter::ScreenResolution);
				if(opt.pic == picPDF) {
					printer.setOutputFormat(QPrinter::PdfFormat);
				} else {
					printer.setOutputFormat(QPrinter::PostScriptFormat);
				}
				printer.setOutputFileName(outfile);

				if(opt.messageLevel == Options::mlVerbose)
					cout << "PDF/EPS area: " << w << ", " << h << endl;

				printer.setFullPage(true);
				printer.setPaperSize(QSizeF(w,h), QPrinter::DevicePixel);

				QPainter painter;
				if(painter.begin(&printer)) {
					painter.translate(-left/*-0.5*/,-top/*-0.5*/);
					processor->draw(painter);
				} else
					cout << "Could not open file " << outfile.toStdString() << endl;
			}
			break;

		default:
			cout << "Picture type not implemented!?" << endl;
		}

		delete processor;
	}

	return 0;
}

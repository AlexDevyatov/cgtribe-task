#ifndef CUSTOMVIEWER_H
#define CUSTOMVIEWER_H
#include <QtWidgets>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/PolygonStipple>
#include <osg/TemplatePrimitiveFunctor>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include <osg/io_utils>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Math>
#include <osg/Timer>

#include <iostream>
#include <windows.h>
#include <math.h>
#include <cassert>
#include <thread>
#include <mutex>

#include "CustomGroup.h"


class CustomViewer : public osgViewer::Viewer, public QWidget
{
public:
	CustomViewer();
	CustomViewer(osg::ref_ptr<CustomGroup> root);
};

#endif
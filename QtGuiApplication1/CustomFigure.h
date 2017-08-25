#ifndef FIGURE_H
#define FIGURE_H

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

#include <QObject>

#include <mutex>

class CustomFigure : public osg::Geometry
{
private:

	osg::ref_ptr<osg::Vec3Array> _vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> _normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> _colors = new osg::Vec4Array;
	double _a;
	double _b;
	double _time;

	int _ka;
	int _kb;

	bool flag = false;

	static bool execute;

	template<class T> T sqr(T x);

	double z(double x, double y, double t);

	std::mutex * mtx;

	void startExecute(int k);

public:

	void FillPoints(osg::Vec3Array* vertices, double l, double r, double step, double time);

	void FillNormals(osg::Vec3Array* vertices, osg::Vec3Array* normals);

	CustomFigure();

	CustomFigure(std::mutex * mtx);

	CustomFigure(osg::ref_ptr<osg::Vec3Array> vertices, osg::ref_ptr<osg::Vec3Array> normals, std::mutex * mtx, double l, double r, double step, double t = 1.0);

	void UpdateAll();

	void SetAll(osg::Vec3Array* vertices, osg::Vec3Array* normals, double l, double r, double step, double t = 1.0);

	void SetAndUpdateVertexArray(osg::Vec3Array* vertices, double l, double r, double step, double t);

	void SetNormalArray(osg::Vec3Array* normals);

	void UpdateNormalArray();

	void UpdateNormalArray(osg::Vec3Array* normals);

	void SetColorArray(osg::Vec4Array* colors);

	bool GetFlag();

	void calc();

	static void StopExecute();

	double GetA();

	double GetB();

	void SetKA(int k);

	void SetKB(int k);

	void SetDefault();

	void SetTime(double ntime);
};
#endif
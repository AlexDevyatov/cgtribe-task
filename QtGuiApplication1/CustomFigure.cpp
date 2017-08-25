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

#include <QSpinBox>

#include <mutex>
#include <thread>
#include <chrono>

#include "CustomFigure.h"

bool CustomFigure::execute = true;

/*double CustomFigure::_a;
double CustomFigure::_b;
int CustomFigure::_ka;
int CustomFigure::_kb;*/

template<class T> T CustomFigure::sqr(T x)
{
	return x * x;
}

double CustomFigure::z(double x, double y, double t)
{
	return sqrt(
		(sqr(_a) * sqr(sin(t)) * sqr(x) + sqr(_b) * sqr(y)) /
		sqr(_a) * sqr(_b) * sqr(sin(t)) + 1);
}

void CustomFigure::FillPoints(osg::Vec3Array* vertices, double l, double r, double step, double t)
{
	for (auto x = l + step; x <= r; x += step)
	{
		vertices->push_back(osg::Vec3(x, l, z(x, l, t)));
		vertices->push_back(osg::Vec3(x - step, l, z(x - step, l, t)));
		vertices->push_back(osg::Vec3(x - step, l + step, z(x - step, l + step, t)));
		for (auto y = l + step; y <= r - step; y += step)
		{
			vertices->push_back(osg::Vec3(x, y, z(x, y, t)));
			vertices->push_back(osg::Vec3(x, y - step, z(x, y - step, t)));
			vertices->push_back(osg::Vec3(x - step, y, z(x - step, y, t)));

			vertices->push_back(osg::Vec3(x, y, z(x, y, t)));
			vertices->push_back(osg::Vec3(x - step, y, z(x - step, y, t)));
			vertices->push_back(osg::Vec3(x - step, y + step, z(x - step, y + step, t)));
		}
		vertices->push_back(osg::Vec3(x, r, z(x, r, t)));
		vertices->push_back(osg::Vec3(x, r - step, z(x, r - step, t)));
		vertices->push_back(osg::Vec3(x - step, r, z(x - step, r, t)));
	}
}

void CustomFigure::FillNormals(osg::Vec3Array* vertices, osg::Vec3Array* normals)
{
	for (int i = 0; i <= vertices->size() - 3; i += 3)
	{
		auto v1 = (*vertices)[i + 1] - (*vertices)[i];
		auto v2 = (*vertices)[i + 2] - (*vertices)[i];
		auto p = v1 ^ v2;
		p.normalize();
		normals->push_back(p);
		normals->push_back(p);
		normals->push_back(p);
	}
}

CustomFigure::CustomFigure()
{
	execute = true;
}

CustomFigure::CustomFigure(std::mutex * mtx)
{
	_a = 2.52;
	_b = 1.0;
	this->mtx = mtx;
	execute = true;
}

CustomFigure::CustomFigure(osg::ref_ptr<osg::Vec3Array> vertices, osg::ref_ptr<osg::Vec3Array> normals, std::mutex * mtx, double l, double r, double step, double t)
{
	this->_time = t;


	FillPoints(vertices, -10, 10, 0.5, t);
	FillNormals(vertices, _normals);

	SetAll(vertices, _normals, -10, 10, 0.5, _time);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	this->_colors = colors;

	_a = 2.52;
	_b = 1.0;
	_ka = _kb = 0;
	this->SetColorArray(colors);

	this->mtx = mtx;
	execute = true;
}

void CustomFigure::UpdateAll()
{
	osg::Geometry::setVertexArray(_vertices.get());
	osg::Geometry::setNormalArray(_normals.get());
	_normals->setBinding(osg::Array::Binding::BIND_PER_VERTEX);
	this->flag = false;
}

void CustomFigure::SetAll(osg::Vec3Array* vertices, osg::Vec3Array* normals, double l, double r, double step, double t)
{
	this->_vertices = vertices;
	this->_normals = normals;
	this->flag = true;
}

void CustomFigure::UpdateNormalArray()
{
	osg::Geometry::setNormalArray(_normals.get());
	_normals->setBinding(osg::Array::Binding::BIND_PER_VERTEX);
}

void CustomFigure::SetNormalArray(osg::Vec3Array* normals)
{
	FillNormals(_vertices, normals);
	this->_normals = normals;
}

void CustomFigure::UpdateNormalArray(osg::Vec3Array* normals)
{
	FillNormals(_vertices, normals);
	this->_normals = normals;
	osg::Geometry::setNormalArray(_normals.get());
	_normals->setBinding(osg::Array::Binding::BIND_PER_VERTEX);
}

void CustomFigure::SetColorArray(osg::Vec4Array* colors) {
	this->_colors = colors;
	osg::Geometry::setColorArray(colors);
	osg::Geometry::setColorBinding(osg::Geometry::BIND_OVERALL);
	osg::Geometry::addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, _vertices->size()));
}

bool CustomFigure::GetFlag()
{
	return this->flag;
}

void CustomFigure::calc()
{
	double lockTime = this->_time;
	while (execute)
	{
		
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array,
			normals = new osg::Vec3Array;
		this->FillPoints(vertices, -10, 10, 0.5, lockTime);
		this->FillNormals(vertices, normals);

		this->mtx->lock();
		auto a = _a + 0.001 * _ka;
		auto b = _b + 0.001 * _kb;
		_a = a;
		_b = b;
		this->SetAll(vertices, normals, -10, 10, 0.5, this->_time);
		lockTime = this->_time;
		this->mtx->unlock();
	}
}

void CustomFigure::StopExecute()
{
	execute = false;
}

double CustomFigure::GetA()
{
	return _a;
}

double CustomFigure::GetB()
{
	return _b;
}

class ScopedLock
{
	std::mutex* _mtx;
public:
	ScopedLock(std::mutex * mtx)
		: _mtx(mtx)
	{
		_mtx->lock();
	}
	~ScopedLock()
	{
		_mtx->unlock();
	}
};

void CustomFigure::SetKA(int k)
{
	ScopedLock sl(mtx);
	_ka = k;
}

void CustomFigure::SetKB(int k)
{
	ScopedLock sl(mtx);
	_kb = k;
}

void CustomFigure::SetDefault()
{
	_ka = 0;
	_kb = 0;
}

void CustomFigure::SetTime(double ntime)
{
	_time = ntime;
}
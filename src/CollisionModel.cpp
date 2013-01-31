#include "CollisionModel.h"
#include "gl_includes.hpp"
#include <cmath>
/**
 * CollisionModel.cpp
 *
 * Author   :   Simon Schwegler
 * Date     :   16.05.2011
 *
 * All rights reserved.
 */

#include <math.h>
#include <iostream>
#include "config.hpp"
using namespace std;

bool debug = false;

CollisionModel::CollisionModel(CollisionType t):
    m_ybottom(0.0), m_ytop(1.0)
{
    if(t == COLL_RECTANGLE)
    {
        //m_vertices = new float[8];
        m_vertices[0] = -1.0; m_vertices[1] = -1.0;
        m_vertices[2] = -1.0; m_vertices[3] = +1.0;
        m_vertices[4] = +1.0; m_vertices[5] = +1.0;
        m_vertices[6] = +1.0; m_vertices[7] = -1.0;
        m_type = t;
    }

    else
    {
        // TODO: throw an exception
        cout << "bad collision type" << endl;
        m_type = COLL_NONE;
    }

}

CollisionModel::~CollisionModel()
{
    //delete [] m_vertices;
}

// return the n'th vertex of the rectangle (transformed to world coords.)
double* CollisionModel::getVertex(int n)
{
    double *nv = new double[2];
    nv[0] = m_vertices[2*n];
    nv[1] = m_vertices[2*n+1];
    return nv;
}

void CollisionModel::setVertex(int n, double x, double z)
{
    m_vertices[2*n] = x;
    m_vertices[2*n + 1] = z;
}

void CollisionModel::printVertices() const
{
    cout << "collmod dump:" << endl;
    cout << m_vertices[0] << " " << m_vertices[1] << endl;
    cout << m_vertices[2] << " " << m_vertices[3] << endl;
    cout << m_vertices[4] << " " << m_vertices[5] << endl;
    cout << m_vertices[6] << " " << m_vertices[7] << endl;
}

// debug method to see the bounding of the collision model
void CollisionModel::draw() const
{
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(m_vertices[0], m_ybottom, m_vertices[1]);
    glVertex3f(m_vertices[2], m_ybottom, m_vertices[3]);
    glVertex3f(m_vertices[4], m_ybottom, m_vertices[5]);
    glVertex3f(m_vertices[6], m_ybottom, m_vertices[7]);
    glVertex3f(m_vertices[0], m_ybottom, m_vertices[1]);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(m_vertices[0], m_ytop, m_vertices[1]);
    glVertex3f(m_vertices[2], m_ytop, m_vertices[3]);
    glVertex3f(m_vertices[4], m_ytop, m_vertices[5]);
    glVertex3f(m_vertices[6], m_ytop, m_vertices[7]);
    glVertex3f(m_vertices[0], m_ytop, m_vertices[1]);
    glEnd();
    glPopMatrix();
}

bool CollisionModel::bCollision(CollisionModel *other)
{
    CollisionType type1 = this->getType();
    CollisionType type2 = other->getType();

    if( (type1 == COLL_RECTANGLE && type2 == COLL_RECTANGLE) )
    {
        // check in x-z plane
        bool xzcoll = RectRect(other);
        // check in y-plane
        float ybottom1, ybottom2, ytop1, ytop2;
        bool ycoll = false;
        if(this->getYBottom() < other->getYBottom())
        {
            ybottom1 = this->getYBottom();
            ybottom2 = other->getYBottom();
            ytop1 = this->getYTop();
            ytop2 = other->getYTop();
        }
        else
        {
            ybottom1 = other->getYBottom();
            ybottom2 = this->getYBottom();
            ytop1 = other->getYTop();
            ytop2 = this->getYTop();
        }
        if(ytop1 >= ybottom2 && ytop1 <= ytop2)           // overlap check
        {
            ycoll = true;
        }
        if(ytop1 >= ytop2)
        {
            ycoll = true;
        }

        if(ycoll == true && xzcoll == true)
            return true;
        else
            return false;
    }

    return false;
}


/**********************************************
// General Helper functions
*********************************************/

void CollisionModel::drawMark(double x, double z, double r, double g, double b)
{
    double scale = 0.1;

    glPushMatrix();
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex3f(x - 1.0*scale, 0.0, z);
    glVertex3f(x, 0.0, z + 1.0*scale);
    glVertex3f(x + 1.0*scale, 0.0, z);
    glVertex3f(x, 0.0, z - 1.0*scale);
    glEnd();
    glPopMatrix();
}

/**********************************************
// Helper functions for Rectangle-Rectangle Collision Detection
*********************************************/

/** TODO: replace the typechecks with asserts */

void drawAxis(double ax, double az);
double *project(double vx, double vz, double ax, double az);
double scalar(double vpx, double vpz, double ax, double az);
double maximum(double vals[4]);
double minimum(double vals[4]);
bool overlap(double s1[4], double s2[4]);

// Rectangle-Rectangle collision detection
bool CollisionModel::RectRect(CollisionModel *other)
{
    // check types
    if(this->getType() != COLL_RECTANGLE  || other->getType() != COLL_RECTANGLE)
    {
        return false;
    }

    // get the axes
    double a[8];
	double *aa = this->getAxis(0);
    a[0] = aa[0];             // this axis
    a[1] = aa[1];
	delete[] aa;
	aa = this->getAxis(1);
    a[2] = aa[0];
    a[3] = aa[1];
	delete[] aa;
	aa = other->getAxis(0);
    a[4] = aa[0];            // other axis
    a[5] = aa[1];
	delete[] aa;
	aa = other->getAxis(1);
    a[6] = aa[0];
    a[7] = aa[1];
	delete[] aa;

    // do overlap check for each axis
    for(int i = 0; i < 4; i++)
    {
        // get current projection axis
        double ax = a[2*i];
        double az = a[2*i + 1];

        // get projections of vertices onto current axis
        double vp1[8];                  // projections of the four vertices from this rect.
        double vp2[8];                  // projections of the four vertices from other rect.
        for(int j = 0; j < 4; j++)
        {
            // project j'th vertex from this rect
            double *v = this->getVertex(j);
            double *vp = project(v[0], v[1], ax, az);
            vp1[2*j] = vp[0];
            vp1[2*j + 1] = vp[1];
			delete[] v, vp;
            // project j'th vertex from other rect
            v = other->getVertex(j);
            vp = project(v[0], v[1], ax, az);
            vp2[2*j] = vp[0];
            vp2[2*j + 1] = vp[1];
			delete[] v, vp;
        }

        // draw the projections
        if(debug)
        {
            for(int j = 0; j < 4; j++)
            {
                drawMark(vp1[2*j], vp1[2*j+1], 1.0, 0.0, 0.0);
                drawMark(vp2[2*j], vp2[2*j+1], 0.0, 1.0, 0.0);
            }
        }

        // get the scalar values of the projected vertices
        double s1[4];                   // scalar values of vertices of this rect.
        double s2[4];                   // scalar values of vertices of other rect.
        for(int j = 0; j < 4; j++)
        {
            s1[j] = scalar(vp1[2*j], vp1[2*j+1], ax, az);
            s2[j] = scalar(vp2[2*j], vp2[2*j+1], ax, az);
        }

        // draw the scalar values
        if(debug)
        {
            for(int j = 0; j < 4; j++)
            {
                drawMark(s1[j], 3.0 + float(i), 1.0, 0.0, 0.0);
                drawMark(s2[j], 3.0 + float(i), 0.0, 1.0, 0.0);
            }
        }

        // if just one axis does not overlap, the rects don't overlap
        if(overlap(s1, s2) == false)
        {
            return false;
        }
    }

    return true;

}

void CollisionModel::drawProjections(double ax, double az, bool drawax, double r, double g, double b)
{
    if(this->getType() != COLL_RECTANGLE)
    {
        return;
    }

    double x[4], z[4];
    for(int i = 0; i < 4; i++)
    {
		double *v = getVertex(i);
        x[i] = v[0];
        z[i] = v[1];
		delete[] v;
    }

    // calculate projections of vertices onto axis
    double xp[4], zp[4];
    for(int i = 0; i < 4; i++)
    {
        xp[i] = (x[i]*ax + z[i]*az)/(ax*ax + az*az) * ax;
        zp[i] = (x[i]*ax + z[i]*az)/(ax*ax + az*az) * az;

		double *vp = project(x[i], z[i], ax, az);
        xp[i] = vp[0];
        zp[i] = vp[1];
		delete[] vp;
    }

    // draw the projected vertices
    for(int i = 0; i < 4; i++)
    {
        drawMark(xp[i], zp[i], r, g, b);
    }
}

void CollisionModel::drawAxes()
{
    if(this->getType() != COLL_RECTANGLE)
    {
        return;
    }

    double *v1 = this->getAxis(0);
    double *v2 = this->getAxis(1);
    drawAxis(v1[0], v1[1]);
    drawAxis(v2[0], v2[1]);
	delete[] v1, v2;
}

double* CollisionModel::getAxis(int which)
{
    if(this->getType() != COLL_RECTANGLE)
    {
        return NULL;
    }

    double x[4], z[4];
    for(int i = 0; i < 4; i++)
    {
		double *v = getVertex(i);
        x[i] = v[0];
        z[i] = v[1];
		delete[] v;
    }

    // compute and return the desired axis vector
	if (which == 0)
	{
		double *v1 = new double[2];
		v1[0] = x[3] - x[0];
		v1[1] = z[3] - z[0];
		return v1;
	}
    else
	{
		double *v2 = new double[2];
		v2[0] = x[2] - x[3];
		v2[1] = z[2] - z[3];
        return v2;
	}
}

double *project(double vx, double vz, double ax, double az)
{
    double *vp = new double[2];
    vp[0] = (vx*ax + vz*az)/(ax*ax + az*az) * ax;
    vp[1] = (vx*ax + vz*az)/(ax*ax + az*az) * az;

    return vp;
}

// get scalar value from projected vertex vectors
double scalar(double vpx, double vpz, double ax, double az)
{
    return (vpx*ax + vpz*az);
}

double maximum(double vals[4])
{
    double maxval = vals[0];
    for(int i = 0; i < 4; i++)
    {
        if(vals[i] > maxval)
            maxval = vals[i];
    }

    return maxval;
}

double minimum(double vals[4])
{
    double minval = vals[0];
    for(int i = 0; i < 4; i++)
    {
        if(vals[i] < minval)
            minval = vals[i];
    }

    return minval;
}

// check if the projected scalar value intervals have an overlap
bool overlap(double s1[4], double s2[4])
{
    double min1 = minimum(s1);
    double max1 = maximum(s1);
    double min2 = minimum(s2);
    double max2 = maximum(s2);

    // swap the intervals s.t. (min1, max1) is the left interval
    if(min2 < min1)
    {
        double temp = min1;
        min1 = min2;
        min2 = temp;
        temp = max1;
        max1 = max2;
        max2 = temp;
    }

    if(max1 < min2)
    {
        return false;
    }

    return true;
}

void drawAxis(double ax, double az)
{
    double st = 10.0;
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.0);
    glBegin(GL_LINES);
    glVertex3f(ax*st, 0.0, az*st);
    glVertex3f(-ax*st, 0.0, -az*st);
    glEnd();
    glPopMatrix();
}

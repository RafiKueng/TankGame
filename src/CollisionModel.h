/**
 * CollisionModel.cpp
 *
 * Author   :   Simon Schwegler
 * Date     :   16.05.2011
 *
 * All rights reserved.
 */

#ifndef COLLISIONMODEL_H
#define COLLISIONMODEL_H

enum CollisionType
{
    COLL_RECTANGLE, COLL_NONE
};

class CollisionModel
{
    public:
        CollisionModel(CollisionType t);
        virtual ~CollisionModel();
        bool bCollision(CollisionModel *other);

        CollisionType getType() {return m_type;}

        double* getVertex(int n);           // return the (transformed) n'th vertex of the rectangle
        void setVertex(int n, double x, double z);
        void printVertices() const;

        void setYTop(float b) {m_ytop = b;}
        float getYTop() const {return m_ytop;}
        void setYBottom(float b) {m_ybottom = b;}
        float getYBottom() const {return m_ybottom;}
        void draw() const;

    protected:

        /**********************************************
        // General Helper functions
        *********************************************/
        void drawMark(double x, double z, double r, double g, double b);

        /**********************************************
        // Helper functions for Rectangle-Rectangle Collision Detection
        *********************************************/
        bool RectRect(CollisionModel *other);
        void drawProjections(double ax, double az, bool drawax, double r, double g, double b);
        void drawAxes();
        double* getAxis(int which);

    private:
        //float *m_vertices;
        float m_vertices[8];
        float m_ytop;
        float m_ybottom;
        CollisionType m_type;
};



#endif // COLLISIONMODEL_H

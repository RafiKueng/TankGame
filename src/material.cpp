/**
 * material.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   08.03.2010
 *
 * All rights reserved.
 */


// Class declaration include
#include "material.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

Material::Material() :
    _ambient( Color( 0.2, 0.2, 0.2, 1.0 ) ),
    _diffuse( Color( 0.8, 0.8, 0.8, 1.0 ) ),
    _specular( Color( 0.0, 0.0, 0.0, 1.0 ) ),
    _emission( Color( 0.0, 0.0, 0.0, 1.0 ) ),
    _shininess( 0 )
{
}

Material::~Material()
{
}

void Material::setActive( GLenum face ) const
{
    float ambientArray[4] = { _ambient.r, _ambient.g, _ambient.b, _ambient.a };
    float diffuseArray[4] = { _diffuse.r, _diffuse.g, _diffuse.b, _diffuse.a };
    float specularArray[4] = { _specular.r, _specular.g, _specular.b, _specular.a };
    float emissionArray[4] = { _emission.r, _emission.g, _emission.b, _emission.a };
    glMaterialfv( face, GL_AMBIENT, ambientArray );
    glMaterialfv( face, GL_DIFFUSE, diffuseArray );
    glMaterialfv( face, GL_SPECULAR, specularArray );
    glMaterialfv( face, GL_EMISSION, emissionArray );
    
    glMaterialf( face, GL_SHININESS, _shininess );
}


GRAPHICSLAB_NAMESPACE_END


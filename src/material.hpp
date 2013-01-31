/**
 * material.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   08.03.2010
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_MATERIAL_HPP
#define GRAPHICSLAB_MATERIAL_HPP

// Includes
#include "types.hpp"
#include "gl_includes.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Class to set up surface material */
class Material
{
public:
	/** Constructor */
    Material();
	/** Destructor */
    ~Material();
    
    /**
	 * Set the material as active
	 *
	 * @param[in]	face	Face to apply the material to
	 */
    void setActive( GLenum face = GL_FRONT_AND_BACK ) const;
    
    // Material properties    
    const Color &getAmbient() const             { return _ambient; }
    void setAmbient( const Color &ambient )     { _ambient = ambient; }
    
    const Color &getDiffuse() const             { return _diffuse; }
    void setDiffuse( const Color &diffuse )     { _diffuse = diffuse; }
    
    const Color &getSpecular() const            { return _specular; }
    void setSpecular( const Color &specular )   { _specular = specular; }
    
    const Color &getEmission() const            { return _emission; }
    void setEmission( const Color &emission )   { _emission = emission; }
    
    const float getShininess() const            { return _shininess; }
    void setShininess( const float shininess )  { _shininess = shininess; }
    
private:
	// Properties of the material
    Color _ambient;
    Color _diffuse;
    Color _specular;
    Color _emission;
    float _shininess;
    
}; // class Material


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_MATERIAL_HPP
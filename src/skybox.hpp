/**
 * Skybox.hpp
 *
 * Author   :   Rafael Küng
 * Date     :   09.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_Skybox_HPP
#define GRAPHICSLAB_Skybox_HPP

// Base class include
#include "drawable.hpp"

// Includes
#include "types.hpp"
#include "tgaTexture.hpp"
#include "material.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

/** Skybox */
class Skybox : public Drawable
{
public:
    /** Constructor */
    Skybox();

    /** Destructor */
    ~Skybox();

    /** Draw the object to the screen. */
    void draw() const;

    /** Reset the Skybox parameters */
    void reset();



private:
	float scale;

	std::string _texNfn; //north
    TGATexture *_texN;
	std::string _texEfn; //north
    TGATexture *_texE;
	std::string _texSfn; //north
    TGATexture *_texS;
	std::string _texWfn; //north
    TGATexture *_texW;
	std::string _texUfn; //north
    TGATexture *_texU;
	std::string _texDfn; //north
    TGATexture *_texD;

	Material _skyMaterial;

}; // class Skybox


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_Skybox_HPP



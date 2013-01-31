/**
 * exception.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   16.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_EXCEPTION_HPP
#define GRAPHICSLAB_EXCEPTION_HPP

// Includes
#include "defs.hpp"

#include <string>

GRAPHICSLAB_NAMESPACE_BEGIN

/**
 * Generic exception class
 */
class Exception
{
public:
    /**
     * Constructor
     *
     * @param[in]   error   Exception error string
     */
    Exception( const std::string &error ) : _error( error ) {}
    
    /** Destructor */
    virtual ~Exception() throw() {}
    
    /**
     * Get the exception error string.
     *
     * @return      Exception error string
     */
    virtual const std::string &what() const throw()  { return _error; }

protected:
    /** Exception error string */
    std::string _error;
    
}; // class Exception


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_EXCEPTION_HPP



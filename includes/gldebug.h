/* MASSIVE THANKS TO THE CHERNO FOR THIS AWESOME TUTORIAL. "https://youtu.be/FBbPWSOQ0-w?si=5iTM2OOHF_eRFdqQ" */

#ifndef GL_DEBUG_H
#define GL_DEBUG_H

#include <glad/glad.h>
#include <assert.h>
#include <iostream>

#define DEBUG // NAME THIS MACRO ANYTHING ELSE EXCEPT "DEBUG" FOR PRODUCTION

#ifdef DEBUG
    #define CHECK(x) ClearFlags();\
    x;\
    assert(LogCall(#x,__FILE__, __LINE__))
#else
    #define CHECK(x) x
#endif

inline std::string PrintErrorMessage(GLenum err){
    if(err == 0x0500){return("GL_INVALID_ENUM : A GLEnum is used in a mismatching context. ");}
    else if(err == 0x0501){return("GL_INVALID_VALUE : A numeric value is out of range. ");}
    else if(err == 0x0502){return("GL_INVALID_OPERATION : Can't perform this operation in this context. ");}
    else if(err == 0x0505){return("GL_OUT_OF_MEMORY : Can't allocate memory, it's full. ");}
    else if(err == 0x0506){return("GL_INVALID_FRAMEBUFFER_OPERATION : Can't read from a framebuffer that is not complete. ");}

    return " ";
} 

inline bool LogCall(const char* function, const char* file, int line){
    while (GLenum err = glGetError()){
        std::cout << "[OpenGL Error] Code: " << err << " " << PrintErrorMessage(err) << 
        "At function: " << function << " : " << file  << std::endl;
        return false;
    }
    return true;
}

inline void ClearFlags(){
    while (glGetError() != GL_NO_ERROR){
        continue;
    }
}
#endif
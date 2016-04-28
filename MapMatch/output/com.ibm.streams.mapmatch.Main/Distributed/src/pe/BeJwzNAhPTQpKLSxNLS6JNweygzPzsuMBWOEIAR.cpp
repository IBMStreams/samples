// eJwzZDRgdCwtyWc0YgxPTQpKLSxNLS4BMYMz87IBdC8IDs


#include "BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR.h"
#include <dst-config.h>
#include <SPL/Runtime/Utility/BackoffSpinner.h>

using namespace SPL;

#define MYPE BeJwzNAhPTQpKLSxNLS6JNweygzPzsuMBWOEIAR


#include <SPL/Runtime/Type/SPLType.h>

static std::vector<std::string> * javaVMArgs() {
    std::vector<std::string> * result = new std::vector<std::string>();

    return result;
}


MYPE::MYPE(bool isStandalone/*=false*/) 
    : SPL::PE(isStandalone, javaVMArgs())
{
   BackoffSpinner::setYieldBehaviour(BackoffSpinner::Auto);
}

MAKE_PE(SPL::MYPE);


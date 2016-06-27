// eJwzZDRkVNKNyK0wMjCxyFVidCwtyQcK_0Qf7BuRn5pX4JpYkZ6QWxRtaWFkBxZzzi4pScxJL8ovijQHOZxGy


#include "BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S.h"
#include <dst-config.h>
#include <SPL/Runtime/Utility/BackoffSpinner.h>

using namespace SPL;

#define MYPE BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0BYo55xcVpeYkluQXxRvHAwD8sg1S


#include <SPL/Runtime/Type/SPLType.h>

static std::vector<std::string> * javaVMArgs() {
    std::vector<std::string> * result = new std::vector<std::string>();

    result->push_back("-Xmx2048m"); 
    return result;
}


MYPE::MYPE(bool isStandalone/*=false*/) 
    : SPL::PE(isStandalone, javaVMArgs())
{
   BackoffSpinner::setYieldBehaviour(BackoffSpinner::Auto);
}

MAKE_PE(SPL::MYPE);


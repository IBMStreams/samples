// eJwzZDRkVNKNyK0wMjCxyFVidCwtyQcK_0Qf7BuRn5pX4JpYkZ6QWxRtaWFkBxSJ8fdxT83NTS4oqg_1NLi5JT4w0BUicUBB


#include "BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf.h"
#include <dst-config.h>
#include <SPL/Runtime/Utility/BackoffSpinner.h>

using namespace SPL;

#define MYPE BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDAyAopF_0Pq4p_0bnppYUVQbnlxYlp8YbxgMAYZkPDf


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


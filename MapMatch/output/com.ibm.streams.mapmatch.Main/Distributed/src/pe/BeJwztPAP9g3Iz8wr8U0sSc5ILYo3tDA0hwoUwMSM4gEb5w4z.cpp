// eJwzYHQsLclnNGT0D_1YNyM_1MK_1FNLEnOSC2KN7SwsoIKFMDEjABTCQ90


#include "BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z.h"
#include <dst-config.h>
#include <SPL/Runtime/Utility/BackoffSpinner.h>

using namespace SPL;

#define MYPE BeJwztPAP9g3Iz8wr8U0sSc5ILYo3tDA0hwoUwMSM4gEb5w4z



MYPE::MYPE(bool isStandalone/*=false*/) 
    : SPL::PE(isStandalone, NULL)
{
   BackoffSpinner::setYieldBehaviour(BackoffSpinner::Auto);
}

MAKE_PE(SPL::MYPE);


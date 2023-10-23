#include "box_base.h"

#ifdef _MSC_VER
#include <boost/locale/encoding.hpp>
#endif

namespace cppbox {

CBase::CBase() {
    
}

CBase::~CBase() {

}

int CBase::getRemainder(int nBase, int nValue) {

    int nRet = nValue % nBase;
    if (nRet < 0) {
        nRet += nBase;
    }
    return nRet;
}

int CBase::getIndex(const int* pData, int nSize, int nValue)
{
    if (!pData) {
        return -1;;
    }

    int nRe = -1;
    for (int i = 0; i < nSize; ++i) {
        if (pData[i] == nValue) {
            nRe = i;
            break;
        }
    }
    return nRe;
}

std::string CBase::u8(const std::string& data) {
#ifdef _MSC_VER
    return boost::locale::conv::between(data, "UTF-8", "GBK");
#else
    return data;
#endif
}

}
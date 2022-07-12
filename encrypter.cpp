#include "encrypter.h"

namespace Encrypt {

CipherCesar::CipherCesar()
{
    m_params["shift"]=m_shift;
    m_params["mult"]=m_mult;
}

void CipherCesar::operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)
{
    result.reserve(resultSize(source.size()));
    uint size=toUnicode(source);
    for(unicode_t &code:m_ucs4)
    {
        for(const auto &diap: *m_diap)
            if(diap(code))
                //return diap.min+(code-diap.min+m_shift)%diap.diff;
                code=diap.min+((code-diap.min)*m_mult+m_shift)%diap.diff;
    }
    result=QString::fromUcs4(m_ucs4.data(), size);
    result.squeeze();
}
//----

CipherAtbash::CipherAtbash()
{
    m_params["shift"]=m_shift;
    m_params["mult"]=m_mult;
}
//---
void CipherAtbash::operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)
{
    result.reserve(resultSize(source.size()));
    uint size=toUnicode(source);
    for(unicode_t &code:m_ucs4)
    {
        for(const auto &diap: *m_diap)
            if(diap(code))
                //return diap.min+(code-diap.min+m_shift)%diap.diff;
                code=diap.max-((code-diap.min)*m_mult+m_shift)%diap.diff;
    }
    result=QString::fromUcs4(m_ucs4.data(), size);
    result.squeeze();
}
} // name space Encrypt

#include "encrypter.h"

namespace Encrypt {

CipherCesar::CipherCesar()
{
    m_type=Cipher::Cesar;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
QString CipherCesar::getDescription()
{
    static QString description=QString(QObject::tr("Affine cipher type: ResultChar=(OriginalChar*mult+shift)%N,\r\n\
   N- is Alphabet size. Here it can be simple as Cesar/Atbash, \r\n\
   where mult param is missed(as equal 1)"));
    return   description;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherCesar::actualize()
{
    if(m_params.contains("shift"))
        m_shift=m_params["shift"].toInt();
    if(m_params.contains("mult"))
        m_mult=m_params["mult"].toInt();
    calcMult();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherCesar::calcMult()
{
    size_t size=m_diap->size();
    int prev=0;
    if(m_multcalc.size()<size)
        m_multcalc.resize(size);
    if(m_mult==1)
    {
        m_multcalc.fill({1,1});
        return;
    }
    for(int i=0;i< size;++i)
    {

        if(1==((m_mult*m_multcalc[i].second )%(*m_diap)[i].diff))
            m_multcalc[i].first=m_mult;
        else if (prev>0 && prev==((*m_diap)[i].diff) && i>0)
        {
            m_multcalc[i].first=m_multcalc[i-1].first;
            m_multcalc[i].second=m_multcalc[i-1].second;
        }
        else
        {
            int target=(*m_diap)[i].diff+1;
            int look=m_mult%((*m_diap)[i].diff);
            int it=look;
            if(it%2==0)
                it--;
            for(look=1;look<target*10;++look)
            if((look*it)%(*m_diap)[i].diff==1)
            {
               m_multcalc[i].first=it;
               m_multcalc[i].second=look;
               break;
            }
        }
        prev=(*m_diap)[i].diff;
    }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherCesar::operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)
{
    result.reserve(resultSize(source.size()));
    uint size=toUnicode(source);
    size_t dsize=m_diap->size();
    for(unicode_t &code:m_ucs4)
    {
//        for(const auto &diap: *m_diap)
        for(int i=0;i< dsize;++i)
        {
            auto diap=(*m_diap)[i];
             if((*m_diap)[i](code))
              {
                //return diap.min+(code-diap.min+m_shift)%diap.diff;
                code=diap.min+((code-diap.min)*m_multcalc[i].first+m_shift)%diap.diff;
                break;
              }
        }
    }
    result=QString::fromUcs4(m_ucs4.data(), size);
    result.squeeze();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherCesar::decrypt(EncrypterBase::text_type& original, const EncrypterBase::text_type& crypted)
{
    original.reserve(resultSize(crypted.size()));
    uint size=toUnicode(crypted);
    size_t dsize=m_diap->size();
    for(unicode_t &code:m_ucs4)
    {
        for(int i=0;i< dsize;++i)
        {
            auto diap=(*m_diap)[i];
            if(diap(code))
              {
                code=diap.min+(((code-diap.min)-m_shift+diap.diff)*m_multcalc[i].second)%diap.diff;
                break;
              }
        }

    }
    original=QString::fromUcs4(m_ucs4.data(), size);
    original.squeeze();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CipherAtbash::CipherAtbash()
{
    m_type=Cipher::Atbash;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherAtbash::operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)
{
    result.reserve(resultSize(source.size()));
    uint size=toUnicode(source);
    size_t dsize=m_diap->size();
    for(unicode_t &code:m_ucs4)
    {
        for(int i=0;i< dsize;++i)
        {
            auto diap=(*m_diap)[i];
            if(diap(code))
              {
                code=diap.max-((code-diap.min)*m_multcalc[i].first+m_shift)%diap.diff;
                break;
              }
        }

    }
    result=QString::fromUcs4(m_ucs4.data(), size);
    result.squeeze();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherAtbash::decrypt(EncrypterBase::text_type& original, const EncrypterBase::text_type& crypted)
{
    original.reserve(resultSize(crypted.size()));
    uint size=toUnicode(crypted);
    int dsize=m_diap->size();
    for(unicode_t &code:m_ucs4)
    {
        for(int i=0;i< dsize;++i)
        {
            auto diap=(*m_diap)[i];
            if(diap(code))
              {
                code=diap.max-(((code-diap.min)+m_shift+diap.diff)*m_multcalc[i].second)%diap.diff;
                break;
              }
        }

    }
    original=QString::fromUcs4(m_ucs4.data(), size);
    original.squeeze();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CipherKeywordCesar::CipherKeywordCesar()
{
    m_type=Cipher::KeywordCesar;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
QString CipherKeywordCesar::getDescription()
{
    static QString description=QString(QObject::tr("Cipher type as Cesar, but shift is non-const,\n\
    and determined by keyword,"));
    return   description;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherKeywordCesar::actualize()
{
   CipherCesar::actualize();
    if(m_params.contains("keyword")){
        m_keyword=m_params["keyword"].toString();
        m_keywordcode=m_keyword.toUcs4();
    }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherKeywordCesar::operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)
{
    uint ksize=m_keywordcode.size();
    if(ksize<=0)
        return CipherKeywordCesar::operator()(source,result);
    uint size=toUnicode(source);
    result.reserve(resultSize(source.size()));
    int idx=0;
    for(unicode_t &code:m_ucs4)
    {
        for(const auto &diap: *m_diap)
        {
            if(diap(code) && diap(m_keywordcode[idx]))
              {
                code=diap.min+((m_keywordcode[idx]+code-2*diap.min)+m_shift)%diap.diff;
                ++idx;
                if(idx>=ksize)
                    idx=0;                 
                break;
              }
        }

    }
    result=QString::fromUcs4(m_ucs4.data(), size);
    result.squeeze();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CipherKeywordCesar::decrypt(EncrypterBase::text_type& original, const EncrypterBase::text_type& crypted)
{
    uint ksize=m_keywordcode.size();
    if(ksize<=0)
        return CipherKeywordCesar::decrypt(original,crypted);
    original.reserve(resultSize(crypted.size()));
    uint size=toUnicode(crypted);
    size_t dsize=m_diap->size();
    uint idx=0;
    for(unicode_t &code:m_ucs4)
    {
        for(int i=0;i< dsize;++i)
        {
            auto diap=(*m_diap)[i];
            if(diap(code) && diap(m_keywordcode[idx]))
              {
                code=diap.min+((code-m_keywordcode[idx])-m_shift+diap.diff)%diap.diff;
                ++idx;
                if(idx>=ksize)
                    idx=0;
                break;
              }
        }

    }
    original=QString::fromUcs4(m_ucs4.data(), size);
    original.squeeze();
}
//---
} // name space Encrypt

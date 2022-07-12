#ifndef ENCRYPTER_H
#define ENCRYPTER_H
//#include "collection.h"
#include <QString>
#include <QVariant>
#include <QMap>

namespace Encrypt {
using unicode_t= unsigned int;
using ParamMap=QMap<QString,QVariant>;
template<typename TextType, typename CharType=decltype(*(TextType::iterator))>
struct EncrypterBase
{
    using text_type=    TextType;
    using char_type=    CharType;
    ParamMap m_params;
    EncrypterBase()=default;
    virtual ~EncrypterBase(){};
    virtual void operator()(const TextType& source, TextType& result)=0;
    //virtual void accept(Visitor* visitor)=0;
    virtual ParamMap getParams(){return m_params;};
    virtual void setParams(ParamMap newparams){ m_params=newparams;};
};
struct Diapazon
{
    unicode_t min,max;
    int diff;
    Diapazon()=delete;
    Diapazon(unicode_t pmin, unicode_t pmax):min(pmin),max(pmax),diff(pmax-pmin){}
    bool operator()(unicode_t code)const {return (code<=max && code>=min);}
};
using Diapazones=std::list<Diapazon>;
//--------------
struct SameSize
{
    uint resultSize(uint inputSize){return inputSize;}
} ;
template<uint Mult=2>
struct MultSize
{
    uint resultSize(uint inputSize){return inputSize*Mult;}
};
using DynamicSize=MultSize<3>;
//-----------
template<typename TextType, typename CharType=decltype(*(TextType::iterator))>
struct EncrypterByDiap : EncrypterBase<TextType,CharType>
{
    const Diapazones *m_diap=nullptr;
public:
    EncrypterByDiap()=default;
    void setDiapazones(Diapazones *diap) { m_diap=diap; };
};
//---
template<typename TextType>
struct UnicodePreProcess
{};
template<>
struct UnicodePreProcess<QString>
{
  QVector<unicode_t> m_ucs4={};
  uint toUnicode(const QString &source)
  {
      m_ucs4=source.toUcs4();
      return m_ucs4.size();
  }
};
//---
//---
struct CipherCesar : EncrypterByDiap<QString,QChar>, SameSize, UnicodePreProcess<QString>
{
    int m_shift=3;
    int m_mult=1;
public:
    CipherCesar();
    virtual void operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)override;
};
struct CipherAtbash : CipherCesar
{
    CipherAtbash();
    virtual void operator()(const EncrypterBase::text_type& source, EncrypterBase::text_type& result)override;
};
} // name space Encrypt
#endif // ENCRYPTER_H

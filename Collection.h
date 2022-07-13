#ifndef COLLECTION_H
#define COLLECTION_H
#include <map>
#include <limits>
#include <QByteArray>
#include <QVariant>
#include "encrypter.h"

namespace Encrypt {


//class QVariant;
using CesarCipher = CipherCesar;
using AffineCipher = CipherCesar;
using CesarCipherAbsolute = CipherCesar;
class Visitor
{
public:
//template<typename Substitution, typename TextType, typename CharType=decltype(*(TextType::iterator))>
//    void visitCipherSubstitution(CipherSubstitution<Substitution,TextType>* obj)
//    {};
    virtual void visit(CesarCipher* obj)=0;
//    virtual void visit(CesarCipherAbsolute* obj)=0;
//    virtual void visit(AffineCipher* obj)=0;
//    virtual void visit(AtbashCipher* obj)=0;
};
//class Setter: Visitor
//{
//    public:
//    ParamMap *m_params=nullptr;
//    void SetParams(EncrypterBase<QString,QChar>* object, ParamMap *params);
////    {
////        m_params=params;
////        object->accept(this);
////    }
//    virtual void visit(CesarCipher* obj)override
//    {
//        obj->m_substitution->m_shift=m_params->operator[]("shift").toInt();
//    };
////    virtual void visit(CesarCipherAbsolute* obj)override
////    {
////        obj->m_substitution->m_shift=(*m_params)["shift"].toInt();
////    };
////    virtual void visit(AffineCipher* obj)override
////    {
////         obj->m_substitution->m_shift=(*m_params)["shift"].toInt();
////         obj->m_substitution->m_advParam=(*m_params)["param1"].toInt();
////         //obj->m_substitution->m_advParam=m_params["param2"];
////    };
//};
class Collection
{
public:
    using Presets = std::map<QString,EncrypterBase<QString,QChar>*>;
protected:
    Presets m_presets;
    Diapazones m_diapazones;
    Diapazones m_diapall={{1,std::numeric_limits<unicode_t>::max()-1}};
    Collection()=default;
    ~Collection()=default;
public:
    static Collection* getCollection()
    {
        static Collection *instance=nullptr;
        if(!instance)
            instance = new Collection;
        return instance;
    }
   bool addPreset(Cipher cipher, ParamMap params, QString name=nullptr);
   void init();
   Diapazones * getDiapazones() {return &m_diapazones;}
   std::map<QString,EncrypterBase<QString,QChar>*>* getPresets() {return &m_presets;}

};
}// Encrypt
#endif // COLLECTION_H



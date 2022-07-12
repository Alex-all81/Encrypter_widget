#include <QVariant>
#include "encrypter.h"
#include "collection.h"

namespace Encrypt {

bool Collection::addPreset(Cipher cipher, ParamMap params, QString name)
{
    EncrypterBase<QString,QChar> *newitem=nullptr;
    if(!name.isEmpty() && m_presets.find(name)!=m_presets.end())
        return false;
    switch (cipher) { //Cesar,Affine,Atbash,CesarAbs
    case Cipher::Cesar:
    case Cipher::CesarAbs:
    case Cipher::Affine:
    {
        if(params.size()<2)
            return false;
        //int shift=params["shift"].toInt();
        newitem = new CipherCesar();
        static_cast<CipherCesar*>(newitem)->setDiapazones(Cipher::CesarAbs==cipher?&m_diapall:getDiapazones());
        newitem->setParams(params);
        break;
    }
    case Cipher::Atbash:
        if(params.size()<2)
            return false;
        newitem = new CipherAtbash();
        static_cast<CipherCesar*>(newitem)->setDiapazones(getDiapazones());
        newitem->setParams(params);
        break;        break;
    default:
        return false;
        break;
    }
    if(newitem )
    {
        m_presets[name]=newitem;
        return true;
    }
   return false;
}
void Collection::init()
{
    ParamMap param;
    param["shift"]=3;
    param["mult"]=1;
    addPreset(Cipher::Cesar,param,"Cesar classic");
    addPreset(Cipher::CesarAbs,param,"Cesar classic absolute");
    addPreset(Cipher::Atbash,param,"Atbash");
    param["mult"]=13;
    addPreset(Cipher::Affine,param,"Affine");
    QString ru="àÿ";
    QString RU="Àß";
    QString en="az";
    QString EN="AZ";
    QVector<uint> ucs4=en.toUcs4();
    m_diapazones.push_back({ucs4[0],ucs4[1]});
    ucs4=EN.toUcs4();
    m_diapazones.push_back({ucs4[0],ucs4[1]});
    ucs4=ru.toUcs4();
    m_diapazones.push_back({ucs4[0],ucs4[1]});
    ucs4=RU.toUcs4();
    m_diapazones.push_back({ucs4[0],ucs4[1]});
}
//void Setter::SetParams(EncrypterBase<QString,QChar>* object, ParamMap *params)
//{
//    m_params=params;
//    object->accept(static_cast<Visitor*>(this));
////    object->
//}
}// Encrypt

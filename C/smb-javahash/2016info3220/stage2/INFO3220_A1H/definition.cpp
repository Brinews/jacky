#include "definition.h"

Definition::Definition(const QString className) :
    m_className(className),
    m_fields(QHash<QString, QString>())
{

}

Definition::~Definition()
{

}

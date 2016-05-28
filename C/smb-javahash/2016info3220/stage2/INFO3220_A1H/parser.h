#ifndef PARSER_H
#define PARSER_H

//enum body_type { planet = 1 };

#include "definition.h"
#include <QFile>
#include <QTextStream>

class Parser
{
public:
    Parser();
    virtual ~Parser();
    QVector<Definition>* readConfigFile(QString filepath);
};

#endif // PARSER_H

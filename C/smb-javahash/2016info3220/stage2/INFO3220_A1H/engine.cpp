#include "engine.h"

#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>

#include "definition.h"

bool Engine::m_exists;
Engine * Engine::m_instance;

Engine::Engine()
{
    m_bodies = new QVector<Body>();
}

Engine::~Engine()
{

}

Engine* Engine::getInstance()
{
    if (!m_exists)
    {
        m_instance = new Engine();
        m_exists = true;
    }
    return m_instance;
}

void Engine::update()
{

}

void Engine::parseConfigFile(const QString filepath,
                             const ComponentFactory& factory)
{
    QFile inputFile(filepath);
    QString regexDelimiters = "[{}:;]";
    QVector<Definition>& definitions = *(new QVector<Definition>);

    // Generate list of Definition files (text-objects)
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        int lineCount = 1;
        bool isClass = false, isField = false;
        QChar lastDelimiter = '}';
        QString token, key;

        while (!in.atEnd())
        {
            QString line = in.readLine(0);

            // Remove whitespace
            line = line.simplified();
            line.replace(" ", "");

            // Parse line, character-by-character
            for (int i = 0; i < line.length(); i++)
            {
                QChar c = line.at(i);

                // Validate config file syntax (see config-file-rules.txt)
                if (QRegExp(regexDelimiters).exactMatch(c))
                {
                    if (c == QChar('{')
                            && !isClass
                            && !isField
                            && lastDelimiter == QChar('}'))
                    {
                        isClass = true;
                        definitions.append(factory.createDefinition(token));
                    }
                    else if (c == QChar('}')
                             && isClass
                             && !isField)
                    {
                        isClass = false;
                    }
                    else if (c == QChar(':')
                             && isClass
                             && !isField
                             && QRegExp("[{;]").exactMatch(lastDelimiter))
                    {
                        isField = true;
                        key = token;
                    }
                    else if (c == QChar(';')
                             && isClass
                             && isField
                             && lastDelimiter == QChar(':'))
                    {
                        isField = false;
                        definitions.last().setField(key, token);
                    }
                    else
                    {
                        qCritical() << "Error parsing line"
                                    << lineCount
                                    << "- unexpected"
                                    << c;
                        exit(1);
                    }
                    lastDelimiter = c;
                    token = ""; // Clear token after successful parse
                }
                else
                {
                    token.append(c);
                }
            }
            lineCount++;
        }

        // Instantiate game components from definitions
        for (int i = 0; i < definitions.size(); i++)
        {
            Definition def = definitions[i];
            m_bodies->append(factory.createBody(def));
        }

        // Clean-up
        inputFile.close();
    }
    else
    {
        qCritical() << "Error reading file - couldn't open"
                    << filepath;
        exit(1);
    }
}

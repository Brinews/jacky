#ifndef DEFINITION_H
#define DEFINITION_H

#include <QHash>
#include <QString>

/*
 * Hash map representation of a config file definition. All values are QStrings.
 * See config-file-rules.txt for more details on how to define an object.
 */
class Definition
{
public:
    Definition(const QString className = "");
    virtual ~Definition();

    // Returns class name
    QString getClassName() const
        { return m_className; }

    // Returns fields associated with given key
    QString getField(const QString key) const
        { return m_fields.value(key); }

    // Returns a list of all keys in this definition
    QList<QString> getKeys() const
        { return m_fields.keys(); }

    // Removes field with associated with given key (does nothing if not found)
    void removeField(const QString key)
        { m_fields.remove(key); }

    // Sets the field in the definition with given key and value
    void setField(const QString key, const QString value)
        { m_fields.insert(key, value); }

protected:
    QString m_className;
    QHash<QString, QString> m_fields;
};

#endif // DEFINITION_H

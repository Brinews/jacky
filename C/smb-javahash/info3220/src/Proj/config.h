#ifndef CONFIG_H
#define CONFIG_H

#include "gamecomponent.h"
#include <list>
#include <string>
#include <unordered_map>


class Config
{
public:
    virtual ~Config();

    //Singleton accessor method
    static Config* getInstance();

    //read the given file
    void read(const std::string& path);

    //use the previously read blocks of text to create the components
    gameComponent* parsegameBlocks();


    //how many times should the screen update each second
    double getFramesPerSecond() const { return m_framesPerSecond; }

    //how many seconds should be simulated each time the physics is updated
    int getPhysicsStepSize() const { return m_physicsStepSize; }

    //split each physics step into this many sub-steps
    int getOvercalculatePhysicsAmount() const { return m_overcalculatePhysics; }

    //meters per pixel, between bodies
    double getDistanceScale() const { return m_distanceScale; }
    void setDistanceScale(double scaleTime) {
        m_distanceScale = m_distScale * scaleTime;
    }


private:
    //private constructor because this is a Singleton
    Config();
    static bool s_initialised;
    static Config* s_instance;

    //read a settings block into the static (class) variables
    void parseSettings();

    //read the all the lines from a file, into a list
    void readLines(const std::string& path, std::list<std::string>&) const;

    //remove a //comment from a line
    std::string stripComment(const std::string& line) const;

    //is the given line a [heading]?
    bool isBlockHeading(const std::string& line) const;

    //is the given line a 'key = value' pair?
    bool isKeyValuePair(const std::string& key, const std::string& op, const std::string& value) const;

    //helper method to join two components (also checks it's an allowed connection)
    void joinComponents(gameComponent* parent, gameComponent* child);


    //member variables to store the parsed text
    std::unordered_map<std::string, std::string> m_settings;
    std::list<std::unordered_map<std::string, std::string> > m_gameBlocks;

    //store a map of {name: component}
    std::unordered_map<std::string, gameComponent*> m_gameComponents;

    double m_framesPerSecond;
    int m_physicsStepSize;
    int m_overcalculatePhysics;
    double m_distanceScale;
    double m_distScale; /* scale base */

};

#endif // CONFIG_H

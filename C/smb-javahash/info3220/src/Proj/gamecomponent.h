#ifndef gameCOMPONENT_H
#define gameCOMPONENT_H

#include <QPainter>
#include <string>

//you probably don't want to change this
#define GRAVITATIONAL_CONSTANT (6.67408e-11)

enum BodySize { small, middle, big };

//enum identifying the type of component
enum gameComponentType { ball, ship, spaceships, space, game };

//forward declaration
class gameBody;

class gameComponent {

public:
    gameComponent(
            gameComponentType type,
            const std::string& name,
            const std::string& parentName = "")
        : m_type(type), m_name(name), m_parentName(parentName) { }
    virtual ~gameComponent() { }

    //composite operation: render the subtree
    virtual void render(QPainter& painter) const = 0;
    virtual void renderLabel(QPainter& painter) const = 0;

    //composite operation: update the positions of all components of this object
    virtual void updatePosition(int timestep) = 0;

    virtual void changeSize() = 0;
    virtual void changeColor() = 0;


    virtual void convertRelativeToAbsolute(double xp, double yp, double xv, double yv) = 0;

    //accessor methods
    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }
    const std::string& getParentName() const { return m_parentName; }
    gameComponentType getType() const { return m_type; }

private:
    gameComponentType m_type;
    std::string m_name;
    std::string m_parentName;

};

#endif // gameCOMPONENT_H

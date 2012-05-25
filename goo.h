#ifndef GOO_H
#define GOO_H

#include <QObject>
#include <QPainter>
#include <QList>

#include <Box2D/Box2D.h>

/*Change FIXED to FIXED_GOO, because it conflict with win32 typedef. Slyshyk*/
enum GooType {NONE,FIXED_GOO,DYNAMIC,REMOVIBLE,BALOON,STICKY};

struct dragInfo{
    b2Vec2 speed;
    float aForce;
    float gScale;
};

class Goo : public QObject
{
    Q_OBJECT
public:
    explicit Goo(int radius=15,QObject *parent = 0);
    //GET TYPE
    GooType getType();

    int nJoints();
    int getMaxJoints();
    int getRadius();
    int getGuestNumber();

    int getDistanceToJoint();

    double getStickness();

    //TREE FUNCTION
    int getNumberOf(GooType type);
    void countingEnd();

    //[not so] obsolete
    bool newGuest();
    bool removeGuest();
    bool canHaveGuest();

    //get target
    Goo* getTarget();


    //Function to drag and drop goo
    virtual void drag();
    void drop();
    void drop(b2Vec2 speed);
    //select unselect goo
    void select(bool s=true);
    bool isSelected();

    //Return function
    virtual bool isDragable();
    bool isMoovable();
    bool isFalling();
    bool isDragging();
    bool hasJoint();
    bool canHaveJoint();
    bool isLinked(Goo* goo);
    bool isOnGround();
    bool isSleeping();

    virtual bool createLink(Goo* goo);
    virtual bool destroyLink(Goo* goo);

    virtual void contactGround()=0; //contact with the ground
    virtual void contactGround(QPoint p)=0; //contact with the ground in the point p
    virtual void destroyThis(); //destroy this goo
    void setTarget(Goo* goo);

    void move(QPoint p);

    void jumpTo(QPoint p);

    virtual void catched();
    virtual void lost();


    b2Body* getBody();
    b2Vec2 getVPosition();
    QPoint getPPosition();

    QList <Goo*> getLinks();
    Goo* getPrevious();
protected:

    GooType type;
    QList <Goo*> links;
    int radius;
    dragInfo info;
    b2Body* body; //physical body

    //TREE VARIABLES
    bool counted;


    //FLAGS
    bool dragable; // If the user can drag it
    bool moovable; // If is moovable
    bool falling; //If is falling down
    bool following;//If is following another goo
    bool dragging; //If the user is courrently dragging that
    bool onGround; //If the goo is on the ground (and inactive)
    bool selected; //know if the goo is selected
    //If the goo is sleeping
    bool sleeping;
    //Propierties
    //contact point
    QPoint groundPoint;
    //limitation of the joints number
    int minJoints,maxJoints;
    //number of guest
    int guestN;
    //maximum number of goo guested on joint
    int maxGuest;
    //max  distance to joint
    int distanceToJoint;

    //value of stick resistance
    double stickness;

    Goo* target,*prevTarget;


    void moveToTarget(); //Move the goos through the joints
    void stopFollow();
    void fallDown();
signals:
    //Signal to retrive a new target.
    void nextTargetPlease(Goo* previous);
    //Signal to advice the the link between a goo is broken (used for the following goo)
    void loseLink(Goo* goo);
    //Signal to destroy this goo
    void destroyGoo();
    //signal to advice to destroy de joint between goo a end goo b
    void destroyJoint(Goo* a,Goo*b);
    //Signal for start to search a neighbor (for sleeping goo)
    void checkForNeighbors(QPoint p);
    //Signal to advice to stop dragging current goo
    void stopDragging();
    //Signal to create a sticky to the ground
    void createSticky(QPoint p);

private slots:
    void checkForConnection(Goo* goo);
   // virtual void deleteSticky()=0;

public slots:
    virtual void paint(QPainter &p)=0;//Draw the goo
    //PaintDebug data
    virtual void paintDebug(QPainter &p)=0;
    virtual void neighborsFound();
};

#endif // GOO_H

Additional Files Required 
Standford Portable Library

spl :- https://drive.google.com/folderview?id=0B3hYfKW5LwT2flg5bW4wUy1oVWFiSE83bTQ5dW44TjFPVGQ2T09lR2ptREZfTmI4V1g4czg&usp=sharing
MakeFile :- https://drive.google.com/file/d/0B3hYfKW5LwT2ODBabXJ1bkV4LWM/view?usp=sharing
spl.jar :- https://drive.google.com/file/d/0B3hYfKW5LwT2WXZqZEl2RnRGaWs/view?usp=sharing

Add the above files in the same directory as the code.


// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRoundRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
G3DRect brick[50];

int main(int argc ,string argv[])
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRoundRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    updateScoreboard(window, label, points);
    double velx=drand48()+3.0;
    double vely=3.0;
    waitForClick();
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                double y = 535;
                setLocation(paddle, x, y);
            }
        }        
        move(ball, velx, vely);
        pause(10);
        if ((getX(ball) + getWidth(ball) >= getWidth(window))||(getX(ball) <= 0))
        {
            velx = -velx;
            pause(10);
        }
        if (getY(ball) <= 0)
        {
            vely = -vely;
            pause(10);
        }
        if(getY(ball) + getWidth(ball) >= getHeight(window))
        {
            double x = (getWidth(window) - 15) / 2;
            double y = (getHeight(window) - 15) / 2;
            setLocation(paddle, 175, 535);
            setLocation(ball,x,y);
            waitForClick();
            lives--;
        }
        GObject object = detectCollision(window, ball);
        if (object == paddle)
        {
           if((getY(ball)+getWidth(ball) >= getY(paddle))&&(getY(ball)+getWidth(ball) <= getY(paddle)+1)&&(getX(ball)+getWidth(ball)/2 >= getX(paddle))&&(getX(ball) <= getX(paddle)+getWidth(paddle)))
           {
           vely=-vely;
           pause(10);
           }
           if((getY(ball) <= getY(paddle) +getHeight(paddle))&&(getX(ball)+getWidth(ball)/2 >= getX(paddle))&&(getX(ball) <= getX(paddle)+getWidth(paddle)))
           {
           vely=vely+0;
           pause(10);
           }
        }
        int z=0;
        for(int i=0;i<ROWS;i++)
        {
           for(int j=0;j<COLS;j++)
           {
             if(object == brick[j+z])
             {
               if((getY(ball)+getWidth(ball) >= getY(brick[j+z]))&&(getX(ball)+getWidth(ball)/2 >= getX(brick[j+z]))&&(getX(ball) <= getX(brick[j+z])+getWidth(brick[j+z])))
               {
                    vely=-vely;
                    removeGWindow(window,brick[j+z]);
                    points++;
                    bricks--;
                    updateScoreboard(window, label, points);
                    pause(10);                    
               }
               else if((getY(ball) <= getY(brick[j+z])+getHeight(brick[j+z]))&&(getX(ball)+getWidth(ball)/2 >= getX(brick[j+z]))&&(getX(ball) <= getX(brick[j+z])+getWidth(brick[j+z])))
               {
                    vely=-vely;
                    removeGWindow(window,brick[j+z]);
                    points++;
                    bricks--;
                    updateScoreboard(window, label, points);
                    pause(10);
               }
               else if((getX(ball)+getWidth(ball) >= getX(brick[j+z]))&&(getY(ball)+getWidth(ball)/2 >= getY(brick[j+z]))&&(getY(ball) <= getY(brick[j+z])+getHeight(brick[j+z])))
               {
                    velx=-velx;
                    removeGWindow(window,brick[j+z]);
                    points++;
                    bricks--;
                    updateScoreboard(window, label, points);
                    pause(10);                 
               }
               else if((getX(ball) <= getX(brick[j+z])+getWidth(brick[j+z]))&&(getY(ball)+getWidth(ball)/2 >= getY(brick[j+z]))&&(getY(ball) <= getY(brick[j+z])+getHeight(brick[j+z])))
               {
                    velx=-velx;
                    removeGWindow(window,brick[j+z]);
                    points++;
                    bricks--;
                    updateScoreboard(window, label, points);
                    pause(10);
               }
                              
             }
           }
           z=z+10;
        }
               
    }
    if(bricks==0)
    {
    double x = (getWidth(window) - 15) / 2;
    double y = (getHeight(window) - 15) / 2;
    setLocation(ball, x,y);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    double y=52.5;
    int z=0;
    for(int i=0;i<ROWS;i++)
    {
       double x=2.5;
       for(int j=0;j<COLS;j++)
       {
           G3DRect bri = newG3DRect(x,y,35,10,5);
           if(i==0)
           {
           setColor(bri,"RED");
           }
           if(i==1)
           {
           setColor(bri,"ORANGE");
           }
           if(i==2)
           {
           setColor(bri,"PINK");
           }
           if(i==3)
           {
           setColor(bri,"GREEN");
           }
           if(i==4)
           {
           setColor(bri,"YELLOW");
           }
           setFilled(bri, true);
           add(window,bri);
           brick[j+z]=bri;
           x=x+40;
       }
       y=y+15;
       z=z+10;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    double x = (getWidth(window) - 15) / 2;
    double y = (getHeight(window) - 15) / 2;
    GOval ball = newGOval( x, y, 15, 15);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRoundRect initPaddle(GWindow window)
{
    GRoundRect paddle=newGRoundRect( 175, 535, 70, 10, 5);
    setColor(paddle, "BLUE");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel scoreboard = newGLabel("");
    setFont(scoreboard, "SansSerif-36");
    add(window, scoreboard);
    return scoreboard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - 2*getHeight(label)- 20) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

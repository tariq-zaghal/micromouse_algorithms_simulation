#include <iostream>
#include <string>
#include <queue>


#include "API.h"


char orientation = 'N'; //mouse orientation

int x = 0; //mouse x coordinate 
int y = 0; //mouse y coordinate

struct mazeNode
{
    /* data */
    short int manhattanDist;
    short int xCoord;
    short int yCoord;
    
    short int wallN = 0;
    short int wallE = 0;
    short int wallS = 0;
    short int wallW = 0;

    short int totalWalls = 0; 

};

mazeNode mazeArray[16][16];


void log(const std::string& text) {
    std::cerr << text << std::endl;
}

void initializeMaze(){
    for(int i = 0; i<16; i++){
        for(int j = 0; j<16; j++){
            mazeArray[i][j].manhattanDist = 1000;
            mazeArray[i][j].xCoord = j;
            mazeArray[i][j].yCoord = i;
            if(j == 0){
                mazeArray[i][j].wallS = 1;
                mazeArray[i][j].totalWalls++;
                API::setWall(i,j,'s');
            }

             if(j == 15){
                 mazeArray[i][j].wallN = 1;
                 mazeArray[i][j].totalWalls++;
                 API::setWall(i,j,'n');
             }

             if(i == 0){
                 mazeArray[i][j].wallW = 1;
                 mazeArray[i][j].totalWalls++;
                 API::setWall(i,j,'w');
             }

            if(i == 15){
                mazeArray[i][j].wallE = 1;
                mazeArray[i][j].totalWalls++;
                API::setWall(i,j,'e');
            }
        }
    }
}

void floodFill(){
    mazeArray[7][7].manhattanDist = 0;
    API::setText(7,7,std::to_string(mazeArray[7][7].manhattanDist));
    mazeArray[7][8].manhattanDist = 0;
    API::setText(7,8,std::to_string(mazeArray[7][8].manhattanDist));
    mazeArray[8][7].manhattanDist = 0;
    API::setText(8,7,std::to_string(mazeArray[8][7].manhattanDist));
    mazeArray[8][8].manhattanDist = 0;
    API::setText(8,8,std::to_string(mazeArray[8][8].manhattanDist));

    std::queue<std::pair<int,int>> q;
    q.push({7, 7});
    q.push({7, 8});
    q.push({8, 7});
    q.push({8, 8});

    //int f = 15000;

    while(!q.empty()){
      //  log(std::to_string(q.size()));
        std::pair<int, int> node = q.front();
        q.pop();
        int x_in = node.first;
        int y_in = node.second; ;


        if(y_in < 15 && mazeArray[x_in][y_in].wallN == 0 && mazeArray[x_in][y_in+1].manhattanDist > mazeArray[x_in][y_in].manhattanDist){
            mazeArray[x_in][y_in+1].manhattanDist = mazeArray[x_in][y_in].manhattanDist + 1;
            API::setText(x_in,y_in+1,std::to_string(mazeArray[x_in][y_in+1].manhattanDist));
            q.push({x_in,y_in+1});
        }

        if(x_in < 15 && mazeArray[x_in][y_in].wallE == 0  && mazeArray[x_in+1][y_in].manhattanDist > mazeArray[x_in][y_in].manhattanDist){
            mazeArray[x_in+1][y_in].manhattanDist = mazeArray[x_in][y_in].manhattanDist + 1;
            API::setText(x_in+1,y_in,std::to_string(mazeArray[x_in+1][y_in].manhattanDist));
            q.push({x_in+1,y_in});
        }

        if(y_in > 0 && mazeArray[x_in][y_in].wallS == 0 && mazeArray[x_in][y_in-1].manhattanDist > mazeArray[x_in][y_in].manhattanDist){
            mazeArray[x_in][y_in-1].manhattanDist = mazeArray[x_in][y_in].manhattanDist + 1;
            API::setText(x_in,y_in-1,std::to_string(mazeArray[x_in][y_in-1].manhattanDist));
            q.push({x_in,y_in-1});
        }

        if(x_in > 0 && mazeArray[x_in][y_in].wallW == 0 && mazeArray[x_in-1][y_in].manhattanDist > mazeArray[x_in][y_in].manhattanDist){
            mazeArray[x_in-1][y_in].manhattanDist = mazeArray[x_in][y_in].manhattanDist + 1;
            API::setText(x_in-1,y_in,std::to_string(mazeArray[x_in-1][y_in].manhattanDist));
            q.push({x_in-1,y_in});
        }            
    }    
}


void updateWalls(){
    if(API::wallFront()){
        if(orientation == 'N' && mazeArray[x][y].wallN == 0){
            mazeArray[x][y].wallN = 1;
            API::setWall(x,y,'n');
            if(y<15){
                mazeArray[x][y+1].wallS = 1;
                API::setWall(x,y+1,'s');
            }
        } else if(orientation == 'S' && mazeArray[x][y].wallS == 0){
            mazeArray[x][y].wallS = 1;
            API::setWall(x,y,'s');
            if(y>0){
                mazeArray[x][y-1].wallN = 1;
                API::setWall(x,y-1,'n');
            }
        } else if (orientation == 'E' && mazeArray[x][y].wallE == 0){
            mazeArray[x][y].wallE = 1;
            API::setWall(x,y,'e');
            if(x<15){
                mazeArray[x+1][y].wallW = 1;
                API::setWall(x+1,y,'w');
            }
        } else if (orientation == 'W' && mazeArray[x][y].wallW == 0){
            mazeArray[x][y].wallW = 1;
            API::setWall(x,y,'w');
            if(x>0){
                mazeArray[x-1][y].wallE = 1;
                API::setWall(x-1,y,'e');
            }
        }
    }

    if(API::wallRight()){
        if(orientation == 'N' && mazeArray[x][y].wallE == 0){
            mazeArray[x][y].wallE = 1;
            API::setWall(x,y,'e');
            if(x<15){
                mazeArray[x+1][y].wallW = 1;
                API::setWall(x+1,y,'w');
            }
        } else if(orientation == 'S' && mazeArray[x][y].wallW == 0){
            mazeArray[x][y].wallW = 1;
            API::setWall(x,y,'w');
            if(x>0){
                mazeArray[x-1][y].wallE = 1;
                API::setWall(x-1,y,'e');
            }
        } else if (orientation == 'E' && mazeArray[x][y].wallS == 0){
            mazeArray[x][y].wallS = 1;
            API::setWall(x,y,'s');
            if(y>0){
                mazeArray[x][y-1].wallN = 1;
                API::setWall(x,y-1,'n');
            }
        } else if (orientation == 'W' && mazeArray[x][y].wallN == 0){
            mazeArray[x][y].wallN = 1;
            API::setWall(x,y,'n');
            if(y<15){
                mazeArray[x][y+1].wallS = 1;
                API::setWall(x,y+1,'s');
            }
        }
    }

    if(API::wallLeft()){
        if(orientation == 'N' && mazeArray[x][y].wallW == 0){
            mazeArray[x][y].wallW = 1;
            API::setWall(x,y,'w');
            if(x>0){
                mazeArray[x-1][y].wallE = 1;
                API::setWall(x-1,y,'e');
            }
        } else if(orientation == 'S' && mazeArray[x][y].wallE == 0){
            mazeArray[x][y].wallE = 1;
            API::setWall(x,y,'e');
            if(x<15){
                mazeArray[x+1][y].wallW = 1;
                API::setWall(x+1,y,'w');
            }
        } else if (orientation == 'E' && mazeArray[x][y].wallN == 0){
            mazeArray[x][y].wallN = 1;
            API::setWall(x,y,'n');
            if(y<15){
                mazeArray[x][y+1].wallS = 1;
                API::setWall(x,y+1,'s');

            }
        } else if (orientation == 'W' && mazeArray[x][y].wallS == 0){
            mazeArray[x][y].wallS = 1;
            API::setWall(x,y,'s');
            if(y>0){
                mazeArray[x][y-1].wallN = 1;
                API::setWall(x,y-1,'n');
            }
        }
    }
}

void printArr(){
    for(int i =0;i<16;i++){
        std::string str1 = "";
        for(int j =0; j<16; j++){
            str1.append(std::to_string(mazeArray[i][j].manhattanDist));
            str1.append("  ");
        }
        log(str1);
    }
}

void move(){
    updateWalls();
 
    if(!API::wallFront() && API::wallRight() && API::wallLeft()){
        API::moveForward(1);
        if(orientation == 'N') y+=1;
        else if(orientation == 'S') y-=1;
        if(orientation == 'E') x+=1;
        else if(orientation == 'W') x-=1;

    }else if(!API::wallFront() && orientation == 'N' && mazeArray[x][y+1].manhattanDist < mazeArray[x][y].manhattanDist){
        API::moveForward(1);
        y+=1;
    }else if(!API::wallFront() && orientation == 'E' && mazeArray[x+1][y].manhattanDist < mazeArray[x][y].manhattanDist){
        API::moveForward(1);
        x+=1;
    }else if(!API::wallFront() && orientation == 'S' && mazeArray[x][y-1].manhattanDist < mazeArray[x][y].manhattanDist){
        API::moveForward(1);
        y-=1;
    }else if(!API::wallFront() && orientation == 'W' && mazeArray[x-1][y].manhattanDist < mazeArray[x][y].manhattanDist){
        API::moveForward(1);
        x-=1;
     }
    else if(!API::wallLeft() && orientation == 'N' && mazeArray[x-1][y].manhattanDist < mazeArray[x][y].manhattanDist){
       // initializeMaze(); //initial distance values and walls of the maze  
       // floodFill();
        orientation = 'W';
        API::turnLeft();
    }else if(!API::wallLeft() && orientation == 'E' && mazeArray[x][y+1].manhattanDist < mazeArray[x][y].manhattanDist){
        //initializeMaze(); //initial distance values and walls of the maze  
        //floodFill();
        orientation = 'N';
        API::turnLeft();
    }else if(!API::wallLeft() && orientation == 'S' && mazeArray[x+1][y].manhattanDist < mazeArray[x][y].manhattanDist){
        //initializeMaze(); //initial distance values and walls of the maze  
        //floodFill();
        orientation = 'E';
        API::turnLeft();
    }else if(!API::wallLeft() && orientation == 'W' && mazeArray[x][y-1].manhattanDist < mazeArray[x][y].manhattanDist){
       // initializeMaze(); //initial distance values and walls of the maze  
       // floodFill();
        orientation = 'S';
        API::turnLeft();
    }

    else if(!API::wallRight() && orientation == 'N' && mazeArray[x+1][y].manhattanDist < mazeArray[x][y].manhattanDist){
       // initializeMaze(); //initial distance values and walls of the maze  
      //  floodFill();
        orientation = 'E';
        API::turnRight();
    }else if(!API::wallRight() && orientation == 'E' && mazeArray[x][y-1].manhattanDist < mazeArray[x][y].manhattanDist){
       // initializeMaze(); //initial distance values and walls of the maze  
       // floodFill();
        orientation = 'S';
        API::turnRight();
    }else if(!API::wallRight() && orientation == 'S' && mazeArray[x-1][y].manhattanDist < mazeArray[x][y].manhattanDist){
       // initializeMaze(); //initial distance values and walls of the maze  
       // floodFill();
        orientation = 'W';
        API::turnRight();
    }else if(!API::wallRight() && orientation == 'W' && mazeArray[x][y+1].manhattanDist < mazeArray[x][y].manhattanDist){
      //  initializeMaze(); //initial distance values and walls of the maze  
       // floodFill();
        orientation = 'N';
        API::turnRight();
    }
    else{
        API::turnRight();
        API::turnRight();

        if(orientation == 'N'){
            orientation = 'S';
        }else if(orientation == 'E'){
            orientation = 'W';
        }else if(orientation == 'S'){
            orientation = 'N';
        }else if(orientation == 'W'){
            orientation = 'E';
        }
        initializeMaze();
        floodFill();
    }

    std::string walls = "";
    walls.append(std::to_string(mazeArray[x][y].wallN));
    walls.append(std::to_string(mazeArray[x][y].wallE));
    walls.append(std::to_string(mazeArray[x][y].wallS));
    walls.append(std::to_string(mazeArray[x][y].wallW));
}


int main(int argc, char* argv[]) {

    initializeMaze(); //initial distance values and walls of the maze  

    floodFill();

    while(!((7<=x && x<=8) && (7<=y && y<= 8)))
        move();

    // while (true)
    // {
    //     move();
    // }

    initializeMaze(); //initial distance values and walls of the maze  
    floodFill();
    
}

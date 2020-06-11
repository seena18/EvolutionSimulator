//final.cpp
//Seena Abed, CISP 400
//12/19/19

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

//Robot class, contains genes and stats for individual robots such as power, rounds survived and energy harvested
class Robot{
private:
  int** genes= new int* [5];

  int power=5;
  int roundC=0;
  int totalE=0;
public:
  Robot(){
    //constructor generates robot's genes;
    for(int i=0;i<5;i++){
      genes[i]=new int[16];
    }
    for(int i=0;i<5;i++){
      for(int j=0;j<16;j++){
          genes[i][j]=rand()%4;
      }
    }

  }
  //bool operator created for the sort function
  bool operator>(const Robot& r) const{
      return(totalE>r.totalE);
  }

    //setters and getters,

  int** getGenes(){
    return genes;
  }
  int getPower(){
    return power;
  }
  void setPower(int p){
    power=p;
  }
  int getCount(){
      return roundC;
  }
  //increments 'rounds' by one
  void incRound(){
      roundC++;
  }
  int getHarvest(){
      return totalE;
  }
  //increments 'harvest energy' by five
  void incHarvest(){
      totalE+=5;
  }

};

//Global Variables- these are for organization
string LINE="  -----------------------------------------";
string LINE1="-----------------------------------------";

//Function prototypes
int squareState(int x,int y,char**);
void moveBot(int c,int &x,int &y,char* grid[10],vector<Robot> &robots,int k);
void displayGrid(char** arr);
void nextGen(vector<Robot> &robots,int size, vector<double> &genFit);


int main() {
  //set random seed, and declare important variables
  srand(time(NULL));
  int menuChoice;
  char userChoice;
  int size=200;

  //Program Greeting
  cout<<endl<<"Robot Evolution Simulator!"<<endl<<endl;
  cout<<"- Watch a population of robots, with randomly generated genes, evolve over many generations as their fitness level increases"<<endl;
  cout<<"- Each robot is put into a walled off 10x10 grid, which is also filled 40% with batteries"<<endl;
  cout<<"- Each robot also starts with a power level of 5 which increases by 5 each time a battery is collected"<<endl;
  cout<<"- After each move, a robot loses a power level. Once it hits 0, the robot dies and the simulation moves to the next robot"<<endl;
  cout<<"- Once each robot has gone, the generation breeds the top 50% of robots and gets rid of the bottom 50%, and then the process repeats"<<endl;
  cout<<"- At the end of the simulation, the average fitness score, calculated by the amount of power collected, will be displayed for each generation"<<endl;
  cout<<"- This way you can see how well each successive generation did"<<endl<<endl;

  //Initial menu, with choices and input validation
  cout<<"What would you like to do"<<endl;
  cout<<" 1. Begin simulation"<<endl;
  cout<<" 2. End program"<<endl;
  cout<<"> ";
  cin>>menuChoice;
  while(menuChoice>1||menuChoice<0){
    cout<<"Incorrect input, try again:";
    cin>>userChoice;
    cout<<endl;
  }
  //User has the option to adjust parameters to their liking, with input validation
  cout<<"The default population size is 200, would you like to change this?(y/n): ";
  cin>>userChoice;
  while(toupper(userChoice)!='Y'&&toupper(userChoice)!='N'){
    cout<<"Incorrect input, try again (y/n):";
    cin>>userChoice;
  }
  cout<<endl;
  if(toupper(userChoice)=='Y'){
      cout<<"What would you like to change the population size to: ";
      cin>>size;
      cout<<endl<<"population size change to: "<<size<<endl;

  }

  //Robot vector declaration for the popultion, this is adjust after each generation. Also parallel genFit vector holds average fitness level of each generation
  vector<Robot> robots(size, Robot());
  vector<double> genFit;
  int genSize=0;
  cout<<endl<<"How many generations would you like to run through: ";
  cin>>genSize;
  cout<<endl;

    cout<<LINE1<<endl;
    //The entirity of this program runs in the following loop for each generation
    for(int z=0;z<genSize;z++){
        cout<<"Running generation #"<<z+1<<endl;
        //This loop runs through each robot of the generation
        for(int k=0;k<size;k++){
        bool moved=false;
        //Following segment creates the board
        char** grid= new char*[10];
        for(int i=0;i<10;i++){
        grid[i]=new char[10];
        }
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
            grid[i][j]=' ';

            }

    }
    //Randomly places 40 batteries on the grid
    for(int i=0;i<40;i++){

        int x=rand()%10;
        int y=rand()%10;
        while(grid[x][y]=='B'){
        x=rand()%10;
        y=rand()%10;
        }
        grid[x][y]='B';
    }
    //Randomly places robot, makes sure there is no battery on that square
    int x=rand()%10;
    int y=rand()%10;
    while(grid[x][y]=='B'){
        x=rand()%10;
        y=rand()%10;
    }

    grid[x][y]='X';

    //The following loop is for each robots run on the board
    while(robots[k].getPower()>0){
        moved=false;
        //Checks for matching genes, and runs the movement code of that gene if it matches
        for(int b=0;b<16;b++){

        if(robots[k].getGenes()[0][b]==squareState(x+1,y,grid)||robots[k].getGenes()[0][b]==3){

            if(robots[k].getGenes()[1][b]==squareState(x-1,y,grid)||robots[k].getGenes()[1][b]==3){
    ;
            if(robots[k].getGenes()[2][b]==squareState(x,y+1,grid)||robots[k].getGenes()[2][b]==3){

                if(robots[k].getGenes()[3][b]==squareState(x,y-1,grid)||robots[k].getGenes()[3][b]==3){

                moveBot(robots[k].getGenes()[4][b],x,y,grid,robots,k);
                moved=true;
                break;

                }
            }
            }
        }


        }
        //if no genes match, movement code 16 runs by default
        if(moved==false){
        moveBot(robots[k].getGenes()[4][16],x,y,grid,robots,k);
        }
        //after each move the robots power decrease by one, and their round count increases by one
        robots[k].setPower(robots[k].getPower()-1);
        robots[k].incRound();

    }

        }
        //calls the nextGen function which creates the next generation
        nextGen(robots,size,genFit);
        //prints the average fitness score for the current generation
        cout<<"Average fitness score: "<<genFit[z]<<endl<<LINE1<<endl;
    }

        return 0;
}



//This function checks which object is in the direction of a sensor and returns corresponding code
int squareState(int x,int y,char* grid[10]){

  if((x<0||x>9)||(y<0||y>9)){

    return 1;
  }
  else if(grid[x][y]==' '){

    return 0;

  }

  else if(grid[x][y]=='B'){

    return 2;
  }
  else{
    return 3;
  }
}

//This function moves a robot depending on what was returned from squareState()
//Also increases power by 5 if battery is detected
void moveBot(int c,int &x,int &y,char* grid[10],vector<Robot> &robots,int k){
  grid[x][y]=' ';
  if(c==0){
    if(x-1>=0){
      x=x-1;
    }

  }
  else if(c==1){
    if(x+1<10){
      x=x+1;
    }
  }
  else if(c==2){
    if(y+1<10){
      y=y+1;
    }
  }
  else if(c==3){
    if(y-1>=0){
      y=y-1;
    }
  }
  if(grid[x][y]=='B'){
    robots[k].setPower(robots[k].getPower()+5);
    robots[k].incHarvest();

  }
  grid[x][y]='X';

}
//Function displays the playing board, was going to create a best run display but I ran out of time
void displayGrid(char** arr){


  cout<<endl;
  cout<<LINE<<endl;
  for(int i=0;i<10;i++){
    if(i<9){
      cout<<i+1<<" ";
    }
    else{
      cout<<i+1;
    }
    for(int j=0;j<10;j++){
      cout<<"|  "<<arr[i][j];
    }
    cout<<"|"<<endl<<LINE<<endl;
  }
}

//This function calculates fitness of each generation and reproduces the top 50% of robots while getting rid of the bottom 50%

void nextGen(vector<Robot> &robots,int size, vector<double> &genFit){

    //calculates average fitness for the generation
    int totalFit=0;
    for(int i=0;i<size;i++){
        totalFit+=robots[i].getHarvest();
    }
    genFit.push_back((double)totalFit/size);

    //sorts robots in descending order
    sort(robots.begin(), robots.end(), greater<Robot>());

    //deletes the bottom 50% of robots
    for(int i=size/2;i<size;i++){
        robots.erase(robots.begin()+i);
    }


    for(int i=0;i<size/2;i+=2){
        //two sibling robots
        Robot r1;
        Robot r2;
        //Parent robots split into chunks of four
        //The following loops alternate the chunks each sibling gets, so that they aren't identical
        for(int k=0;k<4;k+=2){
            for(int b=k*4;b<(k*4)+4;b++){
                for(int a=0;a<5;a++){
                    int mutation=rand()%20;
                    int siblingMut=rand()%2;
                    //5% chance in getting a 1 for each individual element so, thats what determines mutation
                    //randomly selects a sibling to get mutated
                    if(mutation==1){
                        if(siblingMut==0){
                            r1.getGenes()[a][b]=rand()%4;
                            r2.getGenes()[a][b]=robots[i+1].getGenes()[a][b];
                        }
                        else if(siblingMut==1){
                            r2.getGenes()[a][b]=rand()%4;
                            r1.getGenes()[a][b]=robots[i].getGenes()[a][b];
                        }
                    }
                    else{
                        r1.getGenes()[a][b]=robots[i].getGenes()[a][b];
                        r2.getGenes()[a][b]=robots[i+1].getGenes()[a][b];
                    }
                }
            }
        }
        for(int k=1;k<4;k+=2){
            for(int b=k*4;b<(k*4)+4;b++){
                for(int a=0;a<5;a++){
                     int mutation=rand()%20;
                    int siblingMut=rand()%2;
                    if(mutation==1){
                        if(siblingMut==0){
                            r1.getGenes()[a][b]=rand()%4;
                            r2.getGenes()[a][b]=robots[i].getGenes()[a][b];
                        }
                        else if(siblingMut==1){
                            r2.getGenes()[a][b]=rand()%4;
                            r1.getGenes()[a][b]=robots[i+1].getGenes()[a][b];
                        }
                    }
                    else{
                        r1.getGenes()[a][b]=robots[i+1].getGenes()[a][b];
                        r2.getGenes()[a][b]=robots[i].getGenes()[a][b];
                    }
                }
            }
        }
        //sends these new siblings to the end back of the vector
        robots.push_back(r1);
        robots.push_back(r2);

        }




}


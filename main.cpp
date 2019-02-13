#include <iostream>                                   //necessary includes
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef vector <int> ivec;                            //useing typedef to make vector declarations easier
typedef vector <string> svec;
typedef vector <double> dvec;

/*ostream & operator<<(ostream &stream, svec &vec)
 {
 stream << "[";
 for (int i = 0; i < vec.size(); i++)
 {
 stream << vec[i] << " ";                       //code so that I can print out of vectors
 }
 stream << "]";
 
 return stream;
 }
 
 ostream & operator<<(ostream &stream, ivec &vec)
 {
 stream << "[";
 for (int i = 0; i < vec.size(); i++)
 {
 stream << vec[i] << " ";
 }
 stream << "]";
 
 return stream;
 }*/

int stringToInt(string numStr);                       //function prototype
double distPoints(int x1, int y1, int x2, int y2);
double total_Distance(vector <double> distances);

int main()
{
    string filename;                                  //variable for filename
    ifstream infile;
    cout << "Enter the input filename" << endl;
    getline(cin, filename);                           //gets input file from user
    do
    {
        infile.open(filename.c_str(), ifstream::in);
        if(!infile.is_open())                         //if file cannot be opened
        {
            cout<<"Could not open "<<filename<<endl;  //prints error
            cout<<"Enter filename"<<endl;             //gets filename again
            getline(cin, filename);
        }
        else
            break;                                  //if file opens then breaks from loop
    }
    while(true);
    dvec xCoords;
    dvec yCoords;
    int coord_counter = 1;
    while(infile.good())                           //while there are more lines to be read
    {
        int coord;
        string numStr;                                //string to be used to store each line
        infile >> numStr;                      //inputs each line from file into string                                 //code to put points into vectors
        coord = stringToInt(numStr);          //converts string to number
        if(coord_counter % 2 != 0)            //if number is on an odd line
        {
            xCoords.push_back(coord);        //then it is added to the vector of x coordinates
        }
        else if(coord_counter%2==0)         //if the number is on an even line
        {
            yCoords.push_back(coord);       //then it is added to the vector of y coordinates
        }
        coord_counter++;                   //increases the counter to keep track of each line
    }
    if(xCoords.size() > yCoords.size())    //if there are an uneven number of points
    {
        cout<<"There is an error with the coordinates file."<<endl;  //print error
        cout<<"Check that you have an equal number of x and y coordinates, and try again."<<endl; //explain error
        return 0;
    }
    cout<<"Initial Path:"<<endl;                     //statement for clarity
    for(int index=0; index<xCoords.size(); index++)  //loop through all coordinates
    {
        cout<<xCoords[index]<<","<<yCoords[index]<<endl; //print initial order of points (order they were inputted)
    }
    if (xCoords.size() == 1)                         //if there is only one point
    {
        cout<<"Cannot optimize route, as there is only one point."<<endl; //
        return 0;
    }
    else if (xCoords.size() <= 3 && xCoords.size() > 1)
    {
        cout<<"Cannot optimize route as there are not enough points to create multiple unique routes."<<endl;
        cout<<"This order is already optimal."<<endl;
        return 0;
    }
    //pass current path of points in order into total distance calculations
    dvec all_Initial_Distances; //creates vector of initial distances
    for(int x=0; x<xCoords.size(); x++) //loops through all points
    {
        if(x==xCoords.size()-1)   //if this is the last connection
        {
            int exeption = distPoints(xCoords[x], yCoords[x], xCoords[0], yCoords[0]);  //must come back to first point
            all_Initial_Distances.push_back(exeption);   //adds last distance to vector of all distances
            //cout<<"Distance:"<<exeption<<endl;
            break;
        }
        int initial_Distance = distPoints(xCoords[x], yCoords[x], xCoords[x+1], yCoords[x+1]);  //otherwise go from point a to a+1/next point
        all_Initial_Distances.push_back(initial_Distance);  //adds each distance to vector of all distances
        //cout<<"Distance:"<<initial_Distance<<endl;
    }
    double initial_Cost = total_Distance(all_Initial_Distances);    //gets current cost of route
    //cout<<"Total Distance:"<<initial_Cost<<endl;                 //statements for clarity
    double good_Cost;
    good_Cost = initial_Cost;
    double goal_Cost = good_Cost*0.4;                         //sets goal to reduce intial route by 60%
    double  current_Cost = initial_Cost+1;                       //initializes variable called current cost to be greater than the initial cost
    dvec all_Current_Paths;
    int maxProbability = 31128;                         //maximum number of allowable range to adopt worse route
    srand (time(NULL));
    while(current_Cost > goal_Cost || maxProbability <= 10) //add condition for low probability
    {
        int change = rand()% xCoords.size();        //gets random poitions within vector of xCoordinates
        int change2 = rand()% xCoords.size();
        int b = xCoords[change];        //swaps positions in array
        xCoords[change] = xCoords[change2];
        xCoords[change2] = b;
        int a = yCoords[change];        //swaps positions in array
        yCoords[change] = yCoords[change2];
        yCoords[change2] = a;
        //cout<<"size x coordinates:"<<xCoords.size()<<endl;
        
        for(int q=0; q<xCoords.size(); q++) //loops through all points
        {
            if(q==xCoords.size()-1)   //if this is the last connection
            {
                int final_step = distPoints(xCoords[q], yCoords[q], xCoords[0], yCoords[0]);  //must come back to first point
                all_Current_Paths.push_back(final_step);   //adds last distance to vector of all distances
                //cout<<"Distance:"<<final_step<<endl;
                break;
            }
            int current_Path = distPoints(xCoords[q], yCoords[q], xCoords[q+1], yCoords[q+1]);  //otherwise go from point a to a+1/next point
            all_Current_Paths.push_back(current_Path);  //adds each distance to vector of all distances
            //cout<<"Distance:"<<current_Path<<endl;
        }
        
        current_Cost = total_Distance(all_Current_Paths); //finds total distance of new path
        //cout<<"Check cost:"<<current_Cost<<endl;
        
        if (current_Cost < good_Cost)                        //if new route is better
        {
            good_Cost = current_Cost;                        //assigns new route to become default
            //cout<<good_Cost<<endl;
        }
        
        else if (current_Cost >= good_Cost)
        {
            double percentage_Worse = good_Cost/current_Cost;    //for relative odds of acceptance
            double specificProbability = maxProbability * percentage_Worse; //might be wrong and actually make odds beter
            int cap = 32767;                                  //maximum range of generatable nums
            int decider = rand()%cap+1;                      //generates random integer betwe
            if (decider <= specificProbability)
            {
                good_Cost = current_Cost;
            }
            if (decider > specificProbability)
            {
                int b = xCoords[change2];        //swaps positions in array
                xCoords[change2] = xCoords[change];
                xCoords[change] = b;
                int a = yCoords[change2];        //swaps positions in array
                yCoords[change2] = yCoords[change];
                yCoords[change] = a;
            }
        }
        
        all_Current_Paths.clear();
        /*for(int b=0; b<all_Current_Paths.size(); b++)      //delete current paths
         {
         all_Current_Paths.erase(all_Current_Paths.begin()+b);  //delete current paths for next route
         }*/
        
        //cout<<good_Cost<<endl;
        maxProbability-=25;                             //redcues the maximum probability
        if (maxProbability <= 10)                        //if probability is very low
        {
            break;
        }
    }
    cout<<"The new order of the points are:"<<endl;            //formatting statement
    for(int c=0; c<xCoords.size(); c++)                  //loops through current order of coordinates
    {
        cout<<xCoords[c]<<","<<yCoords[c]<<endl;        //prnts x and y coordinates
    }
    cout<<"And then back to the first point."<<endl;
    cout<<"The original random route distance was: "<<initial_Cost<<endl;  //statements for clarity
    cout<<"The new total distance is: "<<good_Cost<<endl;
    float improvement = 100-((good_Cost/initial_Cost)*100);
    cout<<"This is an improvement of "<<improvement<<"%"<<endl;
    return 0;
}

double distPoints(int x1, int y1, int x2, int y2)
{
    double distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));  //caculates the distance between two points
    return distance;                                      //returns distance between the two points
}

double total_Distance(vector <double> distances)
{
    //cout << "size = " << distances.size() << endl;
    double total = 0;                                      //creates a variable for total distance
    for(int i=0; i<distances.size(); i++)              //loops through all distances in vector
    {
        //cout<<distances[i]<<endl;
        total += distances[i];                         //adds them together
    }
    return total;                                      //returns total value of all paths added together
}
int stringToInt(string numStr)
{
    int num=0;
    bool isSuccessful=true;
    int len=numStr.length();
    for(int i=0; i<len; i++)
    {
        char ch=numStr[i];
        int digit;
        if(isdigit(ch))
        {
            stringstream ss;
            ss<<ch;
            ss>>digit;
            num*=10;
            num+=digit;
        }
    }
    if(numStr[0] == '-')
    {
        num=num*-1;
    }
    if(isSuccessful)
    {
        return num;
    }
    return 0;
}




//------------------------------driver.cpp-------------------------------------
// Programmer: Andrea Moore, Rachel Miller, Mat Pollard 
// Class: CSc53 Section C
// Duet Date: 12/04/13
//-----------------------------------------------------------------------------
//Program Description: Program runs the real world business venture simulation.
// It will have customers enter businesses, make purchases and interact in not 
// so friendly manners with one another. Some will go crazy and have to leave 
// the "beautiful" town of Springfield while others will be forced to stay in 
// this godforsaken place! Woohoo! At the end the group who is happiest (people
// who left vs. people who stayed) will be declared the winner!

#include "business.h"
#include "customer.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

const int NUM_PEOPLE = 20;

int main()
{
  //seed random number generator
  srand(time(NULL));
  

  //Variable declarations
  ifstream fin;
  string name;
  string product_name;
  float price;
  int counter = 0; //counts # of times loop has been run
  int inclination; //stores the inclination of the customers
  int goodbye_people = NUM_PEOPLE;
  int hold_rand; //holds random number
  customer hold_cust; //holds customer as array is shuffled
  customer street1[NUM_PEOPLE];//putting the people back into street 
  customer street2[NUM_PEOPLE];//putting the people back into street

  //Create two businesses. Comic book shop and Moe's Bar
  business store1;
  store1 = business("Comic Book Shop", 699.00);
  business store2;
  store2 = business("Moe's Bar", 450.25);
  
  //Access store inventory for Moes Bar
  fin.open("moes.txt");
  for(int i=0; i < STOCK;i++)
  {
    getline(fin, product_name, ',');
    fin>>price;
    
    store2.setInvent(product_name,price,i);
  }
  fin.close();
  
  //Access store inventory for Comic Book Shop
  fin.open("comics.txt");
  for(int i=0; i < STOCK;i++)
  {
    getline(fin, product_name, ',');
    fin>>price;
    fin.ignore(500, '\n');
    store1.setInvent(product_name,price,i);
  }
  fin.close();
  
  //Create array of customers and fill from text file
  customer springfield_residents[NUM_PEOPLE];
  fin.open("people.txt");

  for(int i=0; i < NUM_PEOPLE; i++)
  {
    getline(fin, name, ',');
    fin >> inclination;
    fin.ignore(500, '\n');
    springfield_residents[i] = customer();
    springfield_residents[i].setName(name);
    springfield_residents[i].setInclination(inclination);
  }
  
  fin.close();

  //outputs the initial array of customers
  for(int i=0; i < NUM_PEOPLE; i++)
  {
    cout << springfield_residents[i];
  }

  
  do
  {
    if(counter>=1)
    {
      store1.setNumberPeople(0);
      store2.setNumberPeople(0);
    }
    //Have customers enter store they prefer
    for(int i=0; i < goodbye_people; i++)
    {
      if(springfield_residents[i].getInclination() == -1)
      {
        store2.addCustomer(springfield_residents[i]);
      }
      else
      {
        store1.addCustomer(springfield_residents[i]);
      }
    }

    //Have both stores make sales for all customers
    store1.sellStuff();
    store2.sellStuff();

    //customers leave store
    int spot=0;
    int place=0;
    spot=store1.getNumberPeople();
    place=store2.getNumberPeople();
    store1.customersLeave(street1, spot);
    store2.customersLeave(street2, place);
    for(int i=0; i<spot;i++)
    {  
      springfield_residents[i]=street1[i];
    }
    place+=spot;
    for(int i=spot; i<place;i++)
    { 
      springfield_residents[i]=street2[i-spot];
    }

    //Shuffles array
    for(int i=0; i < goodbye_people; i++)
    {
      hold_rand = rand() % goodbye_people;
      hold_cust = springfield_residents[i];
      springfield_residents[i] = springfield_residents[hold_rand];
      springfield_residents[hold_rand] = hold_cust;
    }
  
    //Make the people interact
    for(int i=0; i < goodbye_people; i++)
    {
      hold_rand = rand() % goodbye_people;
      if(springfield_residents[i].getInclination() == 
        springfield_residents[hold_rand].getInclination() )
      {
        springfield_residents[i].steal(springfield_residents[hold_rand]);	
      }
      else
      {
        springfield_residents[i].deathBeam(springfield_residents[hold_rand]);
      }
    }

    //Goes through array and removes them if happiness is below 10 and above 90
    for(int i=0; i < goodbye_people; i++)
    { 
      if(springfield_residents[i].getHappy() < 10 || 
        springfield_residents[i].getHappy() > 90)
      {
        hold_cust = springfield_residents[i];
        springfield_residents[i] = springfield_residents[goodbye_people-1];
        springfield_residents[goodbye_people-1] = hold_cust;
        cout << springfield_residents[goodbye_people-1].getName();
        cout << " has gone bonkers and have been shipped to ";
        cout << " Shelbyville's House of desperation for your protection." 
        << endl;
        goodbye_people--;
        i--;
      }
      else
      {
        cout << springfield_residents[i].getName() << " isn't crazy!";
        cout << endl;
      }
    }
   
    cout << "=========================================================" <<endl;
    cout << "=========================================================" <<endl;
  
    //Outputs array after each round of interactions  
    /*for(int i=0; i < goodbye_people; i++)
    {
      cout << springfield_residents[i];
    }*/
    counter++;
  }while(goodbye_people > 1 && counter < 20);
  
  //figuring out if moes bar wins or comic book shop wins
  int happymoe=0;
  int happycomic=0;
  for(int i=0; i < NUM_PEOPLE; i++)
  {
    if(springfield_residents[i].getInclination() == -1)
      happymoe+=springfield_residents[i].getHappy();
    else
      happycomic+=springfield_residents[i].getHappy();
  }
 
  //last people standing
  for(int i=0; i<goodbye_people;i++)
    cout<<"the winner is "<<springfield_residents[i]<<" yay!"<<endl;
  
  //Output results of which group has the highest total happiness
  if(happymoe > happycomic)
  {
    cout << "Moe's bar has the happiest customers around..." << endl
         << "Therefore, drunkards shall rule the world eternally." << endl;
    cout << "They have won the Real World Business Adventure!!!" << endl;
    cout << "Comic Book Fans shall now lose most bar fights." << endl;
    cout << "-Throws confetti sarcastically-" << endl;
  }
  else
  {
    cout << "Comic Book Guy has the happiest customers around!" << endl;
    cout << "Every person now has the capacity for +5 Charisma, and familiars"
         <<  " that eat the socks of the drunk." << endl;
    cout << "They have won the Real World Business Adventure!" << endl;
    cout << "Moe's customers eternally search for their socks." << endl;
    cout << "-Throws confetti sarcastically-" << endl;
  }
  return 0;
}


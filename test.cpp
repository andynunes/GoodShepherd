#include <iostream>

using namespace std;

void* pt2Object;        // global variable which points to an arbitrary object

class TClassB
{
public:
   
   void Display(const char* text) { cout << text << endl; };
   static void Wrapper_To_Call_Display(char* text);
   
   /* more of TClassB */
};


// static wrapper-function to be able to callback the member function Display()
void TClassB::Wrapper_To_Call_Display(char* string)
{
   // explicitly cast global variable <pt2Object> to a pointer to TClassB
   // warning: <pt2Object> MUST point to an appropriate object!
   TClassB* mySelf = (TClassB*) pt2Object;
   
   // call member
   mySelf->Display(string);
}


// function does something which implies a callback
// note: of course this function can also be a member function
void DoItB(void (*pt2Function)(char* text))
{
   /* do something */
   
   char tmp[] = "hi, i'm calling back using a global ;-)";
   
   pt2Function(tmp);   // make callback
}


// execute example code
void Callback_Using_Global()
{
   // 1. instantiate object of TClassB
   TClassB objB;
   
   
   // 2. assign global variable which is used in the static wrapper function
   // important: never forget to do this!!
   pt2Object = (void*) &objB;
   
   
   // 3. call 'DoItB' for <objB>
   DoItB(TClassB::Wrapper_To_Call_Display);
}
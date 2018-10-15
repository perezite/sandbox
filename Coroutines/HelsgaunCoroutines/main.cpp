#include "coroutine.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

class A : public Coroutine { void Routine(); }; 
class B : public Coroutine { void Routine(); }; 
class C : public Coroutine { void Routine(); };
Coroutine *a1, *b1, *c1;

void A::Routine() {
    cout << "a1"; Detach();
    cout << "a2"; Call(c1 = new C); 
    cout << "a3"; Call(b1); 
    cout << "a4"; Detach(); 
}

void B::Routine() {
    cout << "b1"; Detach(); 
    cout << "b2"; Resume(c1);
    cout << "b3";
};

void C::Routine() {
    cout << "c1"; Detach();
    cout << "c2" << endl << "==> "; flush(cout);
    char command;
    cin >> command; 
    if (command == 'r') 
        Resume(a1);
    else if (command == 'c') 
        Call(a1);
    else 
        Detach();
    cout << "c3"; Detach();
    cout << "c4"; 
}

void TestProgram() {
    cout << "m1"; Call(a1 = new A);
    cout << "m2"; Call(b1 = new B);
    cout << "m3"; Resume(a1);
    cout << "m4"; Resume(c1); 
    cout << "m5" << endl;
}

int main() Sequencing(TestProgram())

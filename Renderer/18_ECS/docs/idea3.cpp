// Example program
#include <iostream>
#include <string>

using namespace std;

template <class TReturn, class TArg1, class TArg2 = void>
class Function {
    TReturn (*_functionPointer)(TArg1, TArg2);
    
public:
    Function(TReturn(*functionPointer)(TArg1, TArg2) ) : _functionPointer(functionPointer) { }
    
    TReturn execute(const TArg1& arg1, const TArg2& arg2) {
        return _functionPointer(arg1, arg2);
    }
};

template <class TReturn, class TArg1>
class Function<TReturn, TArg1> {
    TReturn (*_functionPointer)(TArg1);
    
public:
    Function(TReturn(*functionPointer)(TArg1) ) : _functionPointer(functionPointer) { }
    
    TReturn execute(const TArg1& arg1) {
        return _functionPointer(arg1);
    }
};

int sum (int a, int b) {
    return a + b;
}

int quad(int a) {
    return a * a;
}

int main()
{
    Function<int, int, int> function1(sum);
    Function<int, int> function2(quad);
    int result1 = function1.execute(1, 2);
    int result2 = function2.execute(4);
    
    cout << result1 << endl;
    cout << result2 << endl;
    
    return 0;
}

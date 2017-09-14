#include "Sample.h" //include the declaration for this class

//<<constructor>>
Sample::Sample(int pin1){
    pin1 = pin1;
}

//<<destructor>>
Sample::~Sample(){/*nothing to destruct*/}

void Sample::setup() {
    //pinMode(pin1, INPUT);
}

// sample method
int Sample::plusone(int i) {
    return sum(i, 1);
}

// sample private method
int Sample::sum(int a, int b) {
    return a + b;
}

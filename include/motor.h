#ifndef MOTOR_H
#define MOTOR_H

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

class Motor
{
    public:

    Motor();
    void setSpeed(unsigned short);
    void step(unsigned short, unsigned char,  unsigned char);
    unsigned char onestep(unsigned char, unsigned char);
    void release(void);

    private:

    unsigned long usperstep;
    unsigned char PWMApin, AIN1pin, AIN2pin;
    unsigned char PWMBpin, BIN1pin, BIN2pin;
    unsigned short revsteps; // # steps per revolution
    unsigned char currentstep;
    unsigned char steppernum;
};

#endif // MOTOR_H

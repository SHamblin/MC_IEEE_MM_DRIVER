//This is header file for MicroMouseLib

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>

#define INT32_MAX   0x7fffffff
#define INT16_MAX   0x7fff
#define INT8_MAX    0x7f

//Generic defines to make functions easer to read
#define I2C_MULTIPLEXER_WRITE 0b11101110
#define I2C_MULTIPLEXER_READ  0b11101111
#define IR_WRITE 0x26  
#define IR_READ  0x27
#define IMU_GYRO_WRITE 0xD6
#define IMU_GYRO_READ  0xD7
#define IMU_MAG_WRITE  0x3C 
#define IMU_MAG_READ   0x3D
#define I2C_MODE_NORMAL TWBR=32;
#define I2C_MODE_FAST   TWBR=2;
#define IR_LEFT        0b00010000
#define IR_FRONT_LEFT  0b00001000//These are both old and un used after we decided we did not need diagnal sensors
#define IR_FRONT       0b00000100
#define IR_FRONT_RIGHT 0b00000010
#define IR_RIGHT       0b00000001

//Buttons == 0 when pressed
#define BUTTON_1 (PINC & 0b00000100)	//On PC2
#define BUTTON_3 (PINC & 0b00001000)	//On PC3
#define LEFT_ENCODER  (PINC & 0b00000001) //On PC0
#define RIGHT_ENCODER (PINC & 0b00000010) //On PC1
#define SIGNAL_READY
#define SIGNAL_NOT_READY

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

//Structs
struct PID_STRUCT{
	float P_Factor;//This stores the P(proportional) term of PID
	float I_Factor;//This stores the intergal portion of PID
	
	int32_t sumError;
	
	int16_t maxError;//This controls the max error, this lets us predict if a P*value will over flow the lint of int16_t
	int32_t maxSumError;
};

//Functions prototype
uint8_t I2CStart(uint8_t address);//Transmits I2C start condition with the address
uint8_t I2CDataSend(uint8_t address);//Sends given data on the I2C bus
uint8_t I2CDataRead(bool more);//Reads data off I2C bus. Only set more to true if there is a read right after
void I2CStop();//Stops and finishes I2C transmission
void I2CPurge(uint8_t cycles);//This purge command toggles the clock line 9 times for each cycle. This will clear junk left in the i2c buffer of i2c devices

void I2CDebugOut(int8_t data);//I2C Debug outputs. Spits data out on the I2C to be grabed by the scope
void I2CDebugOut(uint8_t data);
void I2CDebugOut(int16_t data);
void I2CDebugOut(uint16_t data);

void setUpInital();//Initial basic set up of IO registers
void setUpADC();//Sets up ADC for use by battery alarm
void setUpIMU();//Sets up 9DOF IMU
void IRsensorSelect(uint8_t number);//Function to select the sensor on the I2C bus using the I2C multiplexer
void setupIR();//This function configures the IR sensors. It sets up all 3 connected sensors.
uint16_t readIR(uint8_t sensor);//Reads from a single selected IR sensor
int16_t readGyroZ();//Reads the Z axis of the gyro
float readMagHeading();//Reads the magnetometer heading

void beep();//Simple short beep
void lowBatt();//low battery warning. This is needed since a lipo is damged if it gets bellow 3V per cell
void delayS(unsigned int S);//Easy function for large delays
void sensorTestCalibration();//Diagnostic function to help test sensors at the competition, add more as needed

void pidStructInit(const float PTune, struct PID_STRUCT *pidStruct);//Struct for PID
int16_t pidCalculation(int16_t setPoint, int16_t processValue, struct PID_STRUCT *pidStruct);//Pid calculations

void motorSpeedLeft(int8_t speed);//Old function, -100<->0<->100, limited resolution since it's range is 0-100 int
void motorSpeedRight(int8_t speed);//Old function, -100<->0<->100, limited resolution since it's range is 0-100 int
void motorSpeedBoth(int8_t speedL, int8_t speedR);//Old function to set both motors on a range of -100<->0<->100, Left  Right
void motorSpeedLeft(uint8_t speed, bool reverse);//The motor function use the full range of 0-255 instead of 0-100. This allows more resolution however an extra input is needed to set reverse
void motorSpeedRight(uint8_t speed, bool reverse);
void motorBrake(bool Left, bool Right);//Function to brake each motor

void backAlign();
void moveStraight(int16_t ticks);//Uses ir distance sensors for movement and encoders for distance. This function will move the robot forward in ticks or CM, not sure which yet
void moveStraightGyro();//Move forward using the gyro to keep the bot straight
void moveStraightGyro(uint16_t ticks, bool center = true);//Move forward using the gyro to keep the bot straight and travel a specified ticks
void leftTurnGyro();
void rightTurnGyro();

uint8_t readWalls(uint8_t direction);
uint8_t readWalls2(uint8_t direction);//Walls read function to ignor direction

//Purely for gird based movements
void straight();//Moves One Square
void leftTurn();
void rightTurn();
void turn180();

//These are the direct commands the controler (Pi Zero) will send
void north(uint8_t* heading);
void east(uint8_t* heading);
void south(uint8_t* heading);
void west(uint8_t* heading);

//Depreciated Code-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//void motorTicks(int leftTick,int8_t speed);
//void motorTicksPerSecond(int leftTicksSec, int rightTicksSec);
//void moveStraight2();
//void moveStraight(int16_t distance_CM, int16_t CM_Second_SetPoint);
# include <stdio.h>
# include <time.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);

int main (){
    
    init(1);
    
    char c;
    float kp = 1;
    float ki = 0;
    float kd = 0;
    
    double errorSignal;
    double prop;
    
    double rightMotor;
    double leftMotor;
    int whiteTotal;
    int numberOfWhite;
    int counter = 0;
    
    while(true){
        //initialise values
        if (counter >= 100) { // main exit loop command
		printf("%s\n", "100 loops completed"); // adjust to counter+" loops completed" when you can check C syntax
		break;
	}
        whiteTotal = 0;//sum of the position of the white pixels, measured from center = 0
        numberOfWhite = 0;//running total of the number of white pixels
        
        
        take_picture();
        ////////////////////////////////////////////////////////////////
        display_picture(2,0); //Display picture for debugging purposes
        ////////////////////////////////////////////////////////////////

        for(int i=0; i<320; i++){
        	
            //get pixel "whiteness"
            //resolution of image is 320x240
            c = get_pixel(i,200,3);

            if(c<160){ // 320/2 ie. if (c < half)
                c = 0;  //Black pixel
            }
            else{
                c = 1;   //white pixel
                numberOfWhite++;
            }
            whiteTotal = whiteTotal + (i-160)*c; //add the position of the white pixels (if its white)
        }
	
        if (numberOfWhite == 0){ // prints debug msg & restarts loop if no white
       		printf("%s\n", "No white detected");
       		break; // should become 'cut to maze method' later, will just make a 'motorspeed = 0' method for now
       		counter++;
       	}
        
        if (numberOfWhite >= 1) { // no dividing by 0
        	errorSignal = whiteTotal/numberOfWhite; //center of the white line, running from -160 through 0 to 160
		////////////////////////////////////////////////////////////
		printf("%d\n", errorSignal); //Print error signal for Debugging purposes
		////////////////////////////////////////////////////////////
		counter++;

        } // end of nested loop
        
        
        prop = (errorSignal*127/160);//proportional control
        //the *127/160 scales the value so the motor can handle it
        //equilibrium position: both motors are set to 127

        rightMotor = 127-kp*prop;
        leftMotor = -(127+kp*prop);//negative so motors turn in the same direction

        set_motor(1, rightMotor); //set motor speeds
        set_motor(2, leftMotor);

    } // end of primary loop
    // stop motors
    set_motor(1, 0);
    set_motor(2, 0);
    printf("%s", "\nProgram fin"); // debugging - program complete msg

}

return 0;
}

struct drone
{
    //setpoints for motor
    char rpm1;
    char rpm2;
    char rpm3;
    char rpm4;

    //setpoints for configuration
    char pitch;
    char roll;
    char yawrate;
    char lift;

    //sensed data
    char spitch;
    char sroll;
    char syawrate;

    //flags
    char current_mode;
    char flag_mode;
    char exit;
};

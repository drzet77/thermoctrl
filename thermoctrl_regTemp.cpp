//#include "thermoctrl_regTemp.cpp"

// регулирование температуры двухпозиционным нагревателем
// выход нормально замкнутый, т.е. управл€ющее воздействие - на охлаждение

#define PERIOD_MS 1000
#define OUT_NUM 0
#define OUT_ON 0
#define OUT_OFF 1
#define ACTION_UNDEFINED 0
#define ACTION_HEATING 1
#define ACTION_COOLING -1

main()
{
    new action = ACTION_UNDEFINED;
    new actionOld = ACTION_UNDEFINED;
    while (true)
    {
        new setpoint = GetVar(gIndoorSetpointTemp);
        new temperature = GetVar(gIndoorAverTemp);
        new hysteresis = GetVar(gHysteresis);
        if (hysteresis < 0)
            hysteresis = -hysteresis;

        if (temperature <= (setpoint - hysteresis))
            action = ACTION_HEATING;
        
        if (temperature > setpoint)
            action = ACTION_COOLING;

        if (action != actionOld)
        {
            SetOutputValue(OUT_NUM, action == ACTION_HEATING ? OUT_OFF : OUT_ON);
            actionOld = action;
        }
        Delay(PERIOD_MS);
    }
}
//#include "thermoctrl_averTemp.cpp"

// усреднение температуры

#define TEMPS_AMOUNT_MIN 1
#define TEMPS_AMOUNT_MAX 120
#define PRECISION 1
#define USER_TAG_0_ID 0x62

main()
{
    new temps[TEMPS_AMOUNT_MAX];
    new tempsCount;
    while (true)
    {
        new averagingSteps = GetVar(gAveragingSteps);
        if (averagingSteps < TEMPS_AMOUNT_MIN)
            averagingSteps = TEMPS_AMOUNT_MIN;

        if (averagingSteps > TEMPS_AMOUNT_MAX)
            averagingSteps = TEMPS_AMOUNT_MAX;
            
        if (averagingSteps < tempsCount)
        {
            for (new i = 0; i < averagingSteps; i++)
                temps[i] = temps[(tempsCount - averagingSteps) + i];

            tempsCount = averagingSteps;
        }
        arrayPop(temps, averagingSteps, tempsCount, GetVar(gIndoorTemp));
        for (new i = 0; i < tempsCount; i++)
            Diagnostics("temps[%d]=%d", i, temps[i]);//!!!

        new averTemp = arrayMean(temps, tempsCount, PRECISION);
        SetVar(gIndoorAverTemp, averTemp);
        Diagnostics("averT=%d", averTemp);//!!!
        TagWriteValue(USER_TAG_0_ID, averTemp);
        new averagingStepMs = GetVar(gAveragingStepMs);
        if (averagingStepMs < 0)
            averagingStepMs = 0;

        Delay(averagingStepMs);
    }
}

// !!! в библиотеку
// усреднение подмассива с заданной точностью
arrayMean(const ar[], arSize, precision, start = 0)
{
    if (arSize <= 0)
        return 0;

    if (start < 0)
        start = 0;

    return (arrayPlus(ar, arSize, start) * pow(10, precision)) / (arSize - start);
}

// !!! в библиотеку
//! —уммирование элементов подмассива
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start стартовый индекс подмассива
arrayPlus(const ar[], arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new res;
    for (new i = start; i < arSize; i++)
        res += ar[i];
    
    return res;
}

//! ¬озведение числа в неотрицательную степень,
//! без проверок границ диапазона
//! \param[in] num исходное число
//! \param[in] p степень
pow(num, p)
{
    if (p <= 0)
        return 1;

    new res = num;
    for (new i = 0; i < (p - 1); i++)
        res *= num;

    return res;
}

// !!! в библиотеку
//! ƒобавить в конец с удалением первого и смещением остальных в начало, если не влезал
//! \param[inout] ar массив
//! \param[in] arMaxSize предельный размер массива
//! \param[inout] arSize текущий размер массива
//! \param[in] newElement добавл€емый элемент
arrayPop(ar[], arMaxSize, &arSize, newElement)
{
    if (arMaxSize <= 0)
        return false;

    if (arSize < 0)
        arSize = 0;
    
    if (arSize > arMaxSize)
        arSize = arMaxSize;

    if (arSize < arMaxSize)
    {
        ar[arSize++] = newElement;
        return true;
    }
    for (new i = 0; i < (arSize - 1); i++)
        ar[i] = ar[i + 1];

    ar[arSize - 1] = newElement;
    return true;
}
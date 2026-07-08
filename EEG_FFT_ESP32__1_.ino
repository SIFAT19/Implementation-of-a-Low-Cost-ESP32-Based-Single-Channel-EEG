/*
===========================================================
ESP32 Single Channel EEG Monitor
Version 2.0

Board:
ESP32 DevKit V1

ADC:
GPIO34

Sampling:
500 Hz

FFT:
256 Points

Author:
ChatGPT

===========================================================
*/

#include <Arduino.h>
#include <arduinoFFT.h>

#define EEG_PIN 34

#define SAMPLE_RATE 500
#define FFT_SIZE 128

double vReal[FFT_SIZE];
double vImag[FFT_SIZE];

ArduinoFFT<double> FFT(vReal, vImag, FFT_SIZE, SAMPLE_RATE);

const uint32_t SAMPLE_PERIOD_US = 1000000UL / SAMPLE_RATE;

//----------------------------------------------------
// DC Removal
//----------------------------------------------------

double dc = 0.0;
const double DC_ALPHA = 0.02;

//----------------------------------------------------
// 1st Order High Pass (~1 Hz)
//----------------------------------------------------

double hp_x = 0;
double hp_y = 0;

double highPass(double x)
{
    const double a = 0.9876;

    double y =
        a * (hp_y + x - hp_x);

    hp_x = x;
    hp_y = y;

    return y;
}

//----------------------------------------------------
// 50 Hz Digital Notch (Second Order)
//----------------------------------------------------

double x1=0;
double x2=0;

double notch_y1 = 0;
double notch_y2 = 0;

double notch50(double x)
{
    const double b0 = 0.96508099;
    const double b1 = -1.19328255;
    const double b2 = 0.96508099;

    const double a1 = -1.19328255;
    const double a2 = 0.93016198;

    double y =
        b0 * x +
        b1 * x1 +
        b2 * x2 -
        a1 * notch_y1 -
        a2 * notch_y2;

    x2 = x1;
    x1 = x;

    notch_y2 = notch_y1;
    notch_y1 = y;

    return y;
}

//----------------------------------------------------
// Simple LPF (~35Hz)
//----------------------------------------------------

double lp=0;

double lowPass(double x)
{
    const double alpha=0.30;

    lp += alpha*(x-lp);

    return lp;
}

//----------------------------------------------------

double eegSample=0;

double deltaPower=0;
double thetaPower=0;
double alphaPower=0;
double betaPower=0;
double gammaPower=0;

const double smooth=0.50;

//----------------------------------------------------

void setup()
{

    Serial.begin(500000);

    analogReadResolution(12);

    analogSetPinAttenuation(
        EEG_PIN,
        ADC_11db);

    Serial.println();
    Serial.println("ESP32 EEG Started");

}
//----------------------------------------------------
// MAIN LOOP
//----------------------------------------------------

void loop()
{
    // Collect 256 samples at 500 Hz
    for(int i = 0; i < FFT_SIZE; i++)
    {
        uint32_t t0 = micros();

        double raw = analogRead(EEG_PIN);

        // Remove DC Offset
        dc += DC_ALPHA * (raw - dc);
        raw -= dc;

        // Digital Filters
        raw = highPass(raw);
        raw = notch50(raw);
        raw = lowPass(raw);

        eegSample = raw;

        vReal[i] = raw;
        vImag[i] = 0.0;

        while ((micros() - t0) < SAMPLE_PERIOD_US);
    }

    //------------------------------------------------
    // FFT
    //------------------------------------------------

    FFT.windowing(FFTWindow::Hann, FFTDirection::Forward);

    FFT.compute(FFTDirection::Forward);

    FFT.complexToMagnitude();

    //------------------------------------------------
    // Calculate Band Powers
    //------------------------------------------------

    double delta = 0;
    double theta = 0;
    double alpha = 0;
    double beta  = 0;
    double gamma = 0;

    for(int i=1;i<FFT_SIZE/2;i++)
    {
        double freq =
            ((double)i * SAMPLE_RATE) / FFT_SIZE;

        double power =
            vReal[i] * vReal[i];

        if(freq>=0.5 && freq<4.0)
            delta += power;

        else if(freq>=4.0 && freq<8.0)
            theta += power;

        else if(freq>=8.0 && freq<13.0)
            alpha += power;

        else if(freq>=13.0 && freq<30.0)
            beta += power;

        else if(freq>=30.0 && freq<40.0)
            gamma += power;
    }

    //------------------------------------------------
    // Exponential Smoothing
    //------------------------------------------------

    deltaPower =
        smooth * deltaPower +
        (1.0-smooth) * delta;

    thetaPower =
        smooth * thetaPower +
        (1.0-smooth) * theta;

    alphaPower =
        smooth * alphaPower +
        (1.0-smooth) * alpha;

    betaPower =
        smooth * betaPower +
        (1.0-smooth) * beta;

    gammaPower =
        smooth * gammaPower +
        (1.0-smooth) * gamma;

    //------------------------------------------------
    // Normalize
    //------------------------------------------------

    double total =
        deltaPower +
        thetaPower +
        alphaPower +
        betaPower +
        gammaPower;

    if(total < 1e-15)
        total = 1.0;

    double d = deltaPower / total;
    double t = thetaPower / total;
    double a = alphaPower / total;
    double b = betaPower / total;
    double g = gammaPower / total;
        //------------------------------------------------
    // Print Results
    //------------------------------------------------

    Serial.print("EEG:");
    Serial.print(eegSample, 2);

    Serial.print(" Delta:");
    Serial.print(d, 4);

    Serial.print(" Theta:");
    Serial.print(t, 4);

    Serial.print(" Alpha:");
    Serial.print(a, 4);

    Serial.print(" Beta:");
    Serial.print(b, 4);

    Serial.print(" Gamma:");
    Serial.println(g, 4);

    /*
      Uncomment this block if you want to use
      Arduino Serial Plotter.

      It will plot six traces:
      EEG, Delta, Theta, Alpha, Beta, Gamma

      Example:
      12.5,0.12,0.18,0.32,0.25,0.13
    */

    /*
    Serial.print(eegSample,2);
    Serial.print(",");
    Serial.print(d,4);
    Serial.print(",");
    Serial.print(t,4);
    Serial.print(",");
    Serial.print(a,4);
    Serial.print(",");
    Serial.print(b,4);
    Serial.print(",");
    Serial.println(g,4);
    */

    delay(10);

}   // End loop
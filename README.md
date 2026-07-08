# ESP32-Based Single-Channel EEG Acquisition and Real-Time Brainwave Monitoring System

A low-cost single-channel EEG acquisition system built using an ESP32 microcontroller and a custom analog front-end. The system amplifies microvolt-level EEG signals, removes noise using analog filters, performs real-time FFT processing, and estimates Delta, Theta, Alpha, Beta, and Gamma brainwave activity.

---

## Project Overview

Electroencephalography (EEG) is widely used for neuroscience, biomedical engineering, and Brain–Computer Interface (BCI) applications. Commercial EEG systems are often expensive, limiting their accessibility for students and researchers.

This project demonstrates a compact and affordable EEG monitoring platform that combines analog signal conditioning with embedded digital signal processing.

---

## Features

- Single-channel EEG acquisition
- ESP32-based embedded processing
- AD620 instrumentation amplifier
- Fourth-order Butterworth High-Pass Filter
- Fourth-order Butterworth Low-Pass Filter
- Second-order 50 Hz Active Notch Filter
- Final Gain Stage
- Dual ±5V analog power supply
- Real-time FFT
- Delta, Theta, Alpha, Beta, Gamma extraction
- Arduino Serial Plotter visualization

---

## System Architecture

```
EEG Electrodes
      │
      ▼
Instrumentation Amplifier
      │
      ▼
High-Pass Filter
      │
      ▼
Low-Pass Filter
      │
      ▼
50 Hz Notch Filter
      │
      ▼
Gain Stage
      │
      ▼
DC Bias Network
      │
      ▼
ESP32 ADC
      │
      ▼
FFT
      │
      ▼
Brainwave Band Estimation
      │
      ▼
Serial Plotter
```

---

## Hardware

- ESP32 DevKit V1
- AD620 Instrumentation Amplifier
- TL074 Operational Amplifier
- Ag/AgCl Electrodes
- Passive Components
- Dual ±5V Power Supply

---

## Software

- Arduino IDE
- ArduinoFFT Library

Sampling Frequency

```
500 Hz
```

FFT Size

```
256 Samples
```

Brainwave Bands

| Band | Frequency |
|-------|-----------|
| Delta | 0.5–4 Hz |
| Theta | 4–8 Hz |
| Alpha | 8–13 Hz |
| Beta | 13–30 Hz |
| Gamma | 30–40 Hz |

---

## Results

The system successfully

- Acquires EEG signals in real time
- Amplifies microvolt-level signals
- Suppresses power-line interference
- Estimates EEG frequency bands using FFT
- Displays live EEG activity

Example output

```
EEG: -0.63
Delta: 0.0788
Theta: 0.1126
Alpha: 0.0919
Beta: 0.5279
Gamma: 0.1888
```

---

## Future Work

- PCB implementation
- Multi-channel EEG
- Bluetooth/Wi-Fi streaming
- Mobile application
- Machine Learning-based EEG classification
- Brain–Computer Interface applications

---

## Repository Structure

```
firmware/
hardware/
software/
paper/
images/
docs/
```

---

## Citation

If you use this project, please cite:

S. M. Sifatul Islam,
"Design and Implementation of a Low-Cost ESP32-Based Single-Channel EEG Acquisition and Real-Time Brainwave Monitoring System."

---

## License

MIT License

---

## Author

**S. M. Sifatul Islam**

Department of Electrical and Electronic Engineering

Khulna University of Engineering & Technology (KUET)

Bangladesh

---

⭐ If you found this project useful, consider giving it a star.

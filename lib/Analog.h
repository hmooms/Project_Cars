#ifndef Analog_h
#define Analog_h

#include <list>
#include <Arduino.h>

using namespace std;

/*
 * This class is a replacement eps32 pwm (Analog output).
 * It has will dynamicly spread pins over the avalible channels,
 * the amount of avalible channels depends on the board version:
 *
 * |ESP32 SoC   |Number of LEDC channels|
 * |------------|-----------------------|
 * |ESP32       |16                     |
 * |ESP32-S2    |8                      |(default)
 * |ESP32-C3    |6                      |
 * |ESP32-S3    |8                      |
 *
 * https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/ledc.html
 *
 *
 * use:
 *  Analog::write(pin_number, analog_value);
 *
 */
class Analog
{
public:
    class Channel
    {
    private:
        int channel;
        int value;
        list<int> pins;

    public:
        Channel(int channel, int value, int pin)
        {
            this->channel = channel;
            addPin(pin);
            setValue(value);
        }

        bool contains(int pin)
        {
            for (int channelPin : pins)
            {
                if (pin == channelPin)
                {
                    return true;
                }
            }
            return false;
        }

        int getValue()
        {
            return value;
        }

        void setValue(int value)
        {
            this->value = value;

            ledcWrite(channel, value);
        }

        list<int> getPins()
        {
            return pins;
        }

        void addPin(int pin)
        {
            pins.push_back(pin);
            ledcAttachPin(pin, channel);
        }

        void removePin(int pin)
        {
            pins.remove(pin);
        }
    };

private:
    static vector<Channel> channels;
    static int maxChannels;

    static void addChannel(int pin, int value)
    {
        if (channels.size() >= maxChannels)
            throw; // maybe search closest value or smth

        ledcSetup(channels.size(), 5000, 8); // dunnu about these settings...
        channels.push_back(Channel(channels.size(), value, pin));
    }

public:
    static void write(int pin, int value)
    {

        Channel *currentChannel = NULL;
        Channel *suitableChannel = NULL;
        Channel *emptyChannel = NULL;

        // search channels
        for (size_t i = 0; i < channels.size(); i++)
        {
            if (channels[i].contains(pin))
            {
                if (value == channels[i].getValue())
                    return;

                currentChannel = &channels[i];
            }
            else if (value == channels[i].getValue())
            {
                suitableChannel = &channels[i];
            }
            else if (channels[i].getPins().empty())
            {
                emptyChannel = &channels[i];
            }
        }

        if (currentChannel && currentChannel->getPins().size() == 1)
        {
            currentChannel->setValue(value);
            return;
        }

        if (currentChannel)
            currentChannel->removePin(pin);

        if (suitableChannel)
        {
            suitableChannel->addPin(pin);
            return;
        }

        if (emptyChannel) // should be imposible.. but just in case
        {
            emptyChannel->setValue(value);
            emptyChannel->addPin(pin);
            return;
        }

        addChannel(pin, value);
    }

    void setMaxChannels(int max)
    {
        maxChannels = max;

        // maybe check if amount of live channels > max
    }
};

vector<Analog::Channel> Analog::channels = {};
int Analog::maxChannels = 8;

#endif
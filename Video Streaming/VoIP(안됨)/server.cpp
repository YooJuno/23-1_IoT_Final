#include <boost/asio.hpp>
#include "portaudio.h"
#include <iostream>

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS    (2)

using boost::asio::ip::tcp;

// error handling function
void checkPaError(PaError err) {
    if(err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(1); // terminate in case of error
    }
}

int main() {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 5000));

    tcp::socket socket(io_service);
    boost::system::error_code error;
    acceptor.accept(socket, error);

    if (error) {
        std::cerr << "Accept error: " << error.message() << std::endl;
        return 1;
    }

    // Ignore the first 44 bytes (header of wav file)
    char ignoreBuffer[44];
    boost::asio::read(socket, boost::asio::buffer(ignoreBuffer, sizeof(ignoreBuffer)), error);

    if (error) {
        std::cerr << "Read error: " << error.message() << std::endl;
        return 1;
    }

    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    checkPaError(err);

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "No default output device.\n";
        return 1;
    }

    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    checkPaError(err);

    err = Pa_StartStream(stream);
    checkPaError(err);

    // Reading loop
    try {
        while(true){
            float sampleBlock[FRAMES_PER_BUFFER];

            boost::asio::read(socket, boost::asio::buffer(sampleBlock, sizeof(sampleBlock)), error);

            if (error) {
                std::cerr << "Read error: " << error.message() << std::endl;
                break;
            }

            err = Pa_WriteStream(stream, sampleBlock, FRAMES_PER_BUFFER);
            checkPaError(err);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    err = Pa_StopStream(stream);
    checkPaError(err);

    err = Pa_CloseStream(stream);
    checkPaError(err);

    Pa_Terminate();

    return 0;
}

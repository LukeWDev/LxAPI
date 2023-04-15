#include <utilities/AudioBuffer.hpp>
#include <utilities/AudioSource.hpp>
#include <memory>


using std::unique_ptr;

AudioBuffer::AudioBuffer(const AudioSource& source, int bufferLength, int startSample)
{
    bufferAudioSource = &source;

    const int sampleRate = bufferAudioSource->GetSampleRate();

    buffer.time_s = double(startSample) / sampleRate;
    buffer.samplerate = sampleRate;
    buffer.nch = bufferAudioSource->GetNumChannels();
    buffer.length = bufferLength;
    //unique_ptr<ReaSample> sample = std::make_unique<ReaSample>(uint64_t(buffer.length) * buffer.nch);
    //buffer.samples = sample.get();
    buffer.samples = new ReaSample[static_cast<uint64_t>(buffer.length) * buffer.nch];
    buffer.absolute_time_s = 0.0;

    bufferAudioSource->GetSamples(buffer);
}

AudioBuffer::~AudioBuffer()
{
    delete[] buffer.samples;
}

void AudioBuffer::RefillSamples()
{
    ++bufferFrame;
    buffer.time_s = double(buffer.length) * bufferFrame / buffer.samplerate;
    bufferAudioSource->GetSamples(buffer);
}

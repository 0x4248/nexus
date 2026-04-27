import sounddevice as sd

def pipe_audio(input_device, output_device, sample_rate=44100, channels=(2, 2)):
    """Stream audio from input_device to output_device."""
    def audio_callback(indata, outdata, frames, time, status):
        if status:
            print(status)
        outdata[:] = indata

    with sd.Stream(
        device=(input_device, output_device),
        samplerate=sample_rate,
        channels=channels,
        callback=audio_callback,
    ):
        input("Streaming... press Enter to stop\n")

import sounddevice as sd
import soundfile as sf

def record_audio(input_device, filename="output.wav", sample_rate=44100, channels=2, duration=None):
    """Record audio from input_device into a WAV file.
    
    Args:
        input_device (int): device index
        filename (str): output filename
        sample_rate (int): sample rate
        channels (int): number of channels
        duration (float | None): seconds to record (None = until Enter)
    """
    with sf.SoundFile(filename, mode='w', samplerate=sample_rate,
                      channels=channels) as file:
        def audio_callback(indata, frames, time, status):
            if status:
                print(status)
            file.write(indata)

        with sd.InputStream(device=input_device,
                            samplerate=sample_rate,
                            channels=channels,
                            callback=audio_callback):
            if duration:
                sd.sleep(int(duration * 1000))
            else:
                input(f"Recording to {filename}... press Enter to stop\n")

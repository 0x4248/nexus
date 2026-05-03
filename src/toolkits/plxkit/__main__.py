import argparse
from plxkit.utils import list_devices, get_device_index
from plxkit.pipe import pipe_audio
from plxkit.record import record_audio

def main():
    parser = argparse.ArgumentParser(description="PLXkit audio utilities")
    subparsers = parser.add_subparsers(dest="command")

    # List devices
    subparsers.add_parser("devices", help="List audio devices")

    # Pipe
    pipe_parser = subparsers.add_parser("pipe", help="Pipe audio input → output")
    pipe_parser.add_argument("--in", dest="input", required=False, help="Input device name substring")
    pipe_parser.add_argument("--out", dest="output", required=False, help="Output device name substring")
    pipe_parser.add_argument("--nin", dest="num_input", type=int, default=1, help="Same as --in but using a index, use the devices command to find the index")
    pipe_parser.add_argument("--nout", dest="num_output", type=int, default=1, help="Same as --out but using a index, use the devices command to find the index")

    # Record
    rec_parser = subparsers.add_parser("record", help="Record audio input → file")
    rec_parser.add_argument("--in", dest="input", required=True, help="Input device name substring")
    rec_parser.add_argument("--file", default="output.wav", help="Output file name")
    rec_parser.add_argument("--duration", type=float, help="Duration in seconds (omit for manual stop)")
    rec_parser.add_argument("--nin", dest="num_input", type=int, default=1, help="Same as --in but using a index, use the devices command to find the index")

    args = parser.parse_args()

    if args.command == "devices":
        list_devices()
    elif args.command == "pipe":
        if args.num_input:
            in_dev = args.num_input
        else:
            in_dev = get_device_index(args.input, is_input=True)
        if args.num_output:
            out_dev = args.num_output
        else:
            out_dev = get_device_index(args.output, is_input=False)
        if in_dev is None or out_dev is None:
            print("Error: Could not find matching input/output devices")
            return
        pipe_audio(in_dev, out_dev)
    elif args.command == "record":
        if args.num_input:
            in_dev = args.num_input
        else:
            in_dev = get_device_index(args.input, is_input=True)
        if in_dev is None:
            print("Error: Could not find matching input device")
            return

        record_audio(in_dev, filename=args.file, duration=args.duration)
    else:
        parser.print_help()

if __name__ == "__main__":
    main()

# F1 Packets

Python library for the official F1 game UDP telemetry data

## Packet spec generation

To generate the spec from the official document, follow these steps. Make sure
that `cog` is installed (`pipx install cogapp`) before continuing.

- Copy-paste the documentation into `data/spec.h`
- Comment-out (or delete) anything that is not part of the actual data spec
- Run `cog -Pr .\f1\packets.py` from the root folder.

## Credits

Most of the code is based on
[Telemetry-F1-2021](https://github.com/chrishannam/Telemetry-F1-2021).
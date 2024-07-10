import pickle
from pathlib import Path

from f1.packets import resolve


class PickleListener:
    def __init__(self):
        with (Path(__file__).parent / "packets.pickle").open("rb") as f:
            self.packets = pickle.load(f)

    def __iter__(self):
        yield from (resolve(_) for packets in self.packets.values() for _ in packets)

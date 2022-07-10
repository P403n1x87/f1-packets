import pickle
from pathlib import Path
from f1.packets import resolve


class PickleListener:
    def __init__(self):
        with (Path(__file__).parent / "packets.pickle").open("rb") as f:
            self.packets = pickle.load(f)

    def __iter__(self):
        for i in range(10):
            for key in self.packets:
                yield resolve(self.packets[key][i])

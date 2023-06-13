import pickle
from collections import defaultdict

import f1.listener
from f1.handler import PacketHandler
from f1.listener import PacketListener


class PacketRecorder(PacketHandler):
    def __init__(self, listener):
        super().__init__(listener)
        self._packets = defaultdict(list)

        self.resolve = f1.listener.resolve
        f1.listener.resolve = self._resolve

    def _resolve(self, packet):
        resolved = self.resolve(packet)

        packet_list = self._packets[type(resolved).__name__]
        if len(packet_list) < 1000:
            packet_list.append(packet)
            print(f"Added packet {len(packet_list)} to {type(resolved).__name__}")

        return resolved


handler = PacketRecorder(PacketListener())

try:
    handler.handle()
except KeyboardInterrupt:
    with open("test/packets.pickle", "wb") as f:
        pickle.dump(handler._packets, f)
    print("Data recording completed")

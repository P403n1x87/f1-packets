from f1.listener import PacketListener


class PacketHandler:
    def __init__(self, listener: PacketListener):
        self.listener = listener

    def handle_generic(self, packet):
        pass

    def handle(self):
        for packet in self.listener:
            self.handle_generic(packet)

            name = packet.__class__.__name__
            if name.startswith("Packet"):
                name = name[6:]
            handler = getattr(self, f"handle_{name}", None)
            if handler is not None:
                handler(packet)

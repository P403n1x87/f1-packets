"""
Basic listener to read the UDP packet and convert it to a known packet format.
"""

import platform
import socket

from f1.packets import resolve


class PacketListener:
    def __init__(self, host: str = "", port: int = 20777):
        self.socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        if platform.system() == "Windows":
            self.socket.settimeout(0.5)
        self.socket.bind((host, port))

    def get(self):
        while True:
            try:
                return resolve(self.socket.recv(2048))
            except socket.timeout:
                pass

    def __iter__(self):
        while True:
            yield self.get()

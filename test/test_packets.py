from test.utils import PickleListener


def test_packet_unpacking():
    listener = PickleListener()
    assert len(list(listener)) == len(listener.packets) * 10

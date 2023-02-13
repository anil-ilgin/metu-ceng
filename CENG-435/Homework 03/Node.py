import sys
import socket
import selectors
import time
import types
import os
import pickle
import math

neighbours = {}
HOST = "127.0.0.1"
PORT = int(sys.argv[1])
f = open("{}/{}.costs".format(os.getcwd(), PORT), 'r')
N_NODE = int(f.readline())
starting_ports = []
clients = []
socks = []
sel = selectors.DefaultSelector()


def printer():
    ls = sorted(list(neighbours.items()))
    output = ""
    for elem in ls:
        output += "{} -{} | {}\n".format(PORT, elem[0], elem[1])
    print(output)


def initialize():
    global N_NODE, neighbours, starting_ports
    for i in range(3000, 3000+N_NODE):
        if i != int(PORT):
            neighbours[i] = math.inf
        else:
            neighbours[i] = 0

    for line in f.readlines():
        line = line.strip('\n').split(' ')
        neighbours[int(line[0])] = int(line[1])
        starting_ports.append(int(line[0]))


def calc_dist_vector(rec_vec):
    global timer
    dummy_dict = rec_vec
    update = False
    fromport = 0
    for key in dummy_dict.keys():
        if dummy_dict[key] == 0:
            fromport = key

    for key in dummy_dict.keys():
        minval = min(dummy_dict[key] + neighbours[fromport], neighbours[key])
        if minval < neighbours[key]:
            neighbours[key] = minval
            update = True
            timer = time.time()

    return update


def accept_socket(conn,addr):
    data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
    clients.append(data)
    sel.register(conn, selectors.EVENT_READ | selectors.EVENT_WRITE, data=data)


def reader(sock):
    recv_data = sock.recv(4096)
    if recv_data:
        recieved = pickle.loads(recv_data)
        val = calc_dist_vector(recieved)
        if val:
            for c in clients:
                c.outb += pickle.dumps(neighbours)

def writer(sock, data):
    sent = sock.send(data.outb)
    data.outb = data.outb[sent:]

def start_connections():
    for p in starting_ports:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.setblocking(False)
        sock.connect_ex((HOST, p))
        socks.append(sock)
        data = types.SimpleNamespace(outb=b"")
        data.outb = pickle.dumps(neighbours)
        sel.register(sock, selectors.EVENT_READ | selectors.EVENT_WRITE, data=data)



def main():
    initialize()
    lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    lsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    lsock.bind((HOST, PORT))
    lsock.listen()
    lsock.setblocking(False)
    time.sleep(1)
    sel.register(lsock, selectors.EVENT_READ, data=None)
    start_connections()
    time.sleep(1)

    timer = time.time()
    while time.time() - timer < 5:
        events = sel.select(timeout=None)
        for key, mask in events:
            if key.data is None:
                conn, addr = key.fileobj.accept()
                conn.setblocking(False)
                accept_socket(conn,addr)
            else:
                if mask & selectors.EVENT_READ:
                    reader(key.fileobj)
                if mask & selectors.EVENT_WRITE:
                    if key.data.outb:
                        writer(key.fileobj, key.data)
    sel.close()
    lsock.close()
    for elem in socks:
        elem.close()

main()
printer()
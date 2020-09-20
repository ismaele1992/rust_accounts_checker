
import valve.source.master_server
import valve.source.a2s
import valve.source.messages
import socket
import time
from six import unichr


def get_server():
    with valve.source.master_server.MasterServerQuerier() as msq:
        values = msq.find(region=["all"], duplicates="skip", gamedir="rust", empty="false")
        number_servers = 0
        for v in values:
            print(v)
            #print(str(v[0]))
            #get_server_info(str(v[0]), int(v[1]))
            number_servers += 1
        print("Found {0} servers".format(number_servers))


def get_server_info(ipaddress=None, port=None):
    with valve.source.a2s.ServerQuerier((ipaddress, port), timeout=60.0) as a2:
        for a in a2.info():
            print("{0} : {1}".format(a, a2.info()[a]))
        print(a2.players())
        for a in a2.players()['players']:
            for b in a:
                print("{0} : {1}".format(b, a[b]))
            print("\n")


def process_output(message):
    print("Processing output...")
    print("Lenght of the package = {0}".format(len(message)))
    message = message[5:]
    number_players = int(message[0])
    print("Number of players {0}".format(number_players))
    t_type = 0
    player = ""
    otherparams = 0
    number_players_out = 0
    with open('players.txt', 'w') as p_file:
        for x in range(0, len(message[1:])):
            if t_type == 0:
                if message[x] == 0:
                    #print("ID player {0}".format(message[x]))
                    t_type = 1
            elif t_type == 1:
                if message[x] != 0:
                    #print("Int : ", message[x])
                    #print("Bytes : ", unichr(message[x]))
                    #print("Decoded : ", bytes(message[x]).decode())
                    player += chr(message[x])
                elif message[x] == 0:
                    t_type = 2
                    print("Player name {0}".format(player))
                    #p_file.write(player + '\n')
                    number_players_out += 1
                    player = ""
            elif t_type == 2:
                if otherparams < 7:
                    otherparams += 1
                else:
                    otherparams = 0
                    t_type = 0
    if number_players_out == number_players:
        print("Number of players are right!")


def send_up(ip_server, port_server):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #s.sendto(b'\xff\xff\xff\xff\x55\xff\xff\xff\xff', (ip_server, port_server))
    message = b'\xff\xff\xff\xff\x55\xff\xff\xff\xff'
    print("Sending {0} to {1}".format(message, ip_server))
    s.sendto(b'\xff\xff\xff\xff\x55\xff\xff\xff\xff', (ip_server, port_server))
    a = s.recv(8192)
    print(a)
    time.sleep(0.5)
    message = a[0:4] + b'\x55' + a[5:9]
    s.sendto(message, (ip_server, port_server))
    print("Sending {0} to {1}".format(message, ip_server))
    a = s.recv(8192)
    print(a)
    time.sleep(0.5)
    print("AAAA", a[4])
    while a[4] == 65:
        message = a[0:4] + b'\x55' + a[5:9]
        print("Sending {0} to {1}".format(message, ip_server))
        s.sendto(message, (ip_server, port_server))
        a = s.recv(8192)
        time.sleep(0.5)
        print(a)
    process_output(a)


if __name__ == "__main__":
    #get_server()
    #get_server_info("164.132.207.192", 28015)
    #get_server_info("145.239.130.174", 28015)
    send_up("145.239.130.174", 28015)
    #send_up("43.229.79.205", 10001)
    #send_up("145.239.205.157", 28016)

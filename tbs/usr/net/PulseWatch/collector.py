import socket
import time

def _read_kv(path):
    out = {}
    with open(path) as f:
        for line in f:
            k, *v = line.split()
            out[k.rstrip(":")] = int(v[0])
    return out

def cpu_usage():
    with open("/proc/stat") as f:
        cpu = list(map(int, f.readline().split()[1:]))

    idle = cpu[3] + cpu[4]
    total = sum(cpu)
    return idle, total

_prev_idle, _prev_total = cpu_usage()

def cpu_percent():
    global _prev_idle, _prev_total
    idle, total = cpu_usage()

    didle = idle - _prev_idle
    dtotal = total - _prev_total

    _prev_idle, _prev_total = idle, total
    return round(100 * (1 - didle / dtotal), 2)

def mem_info():
    m = _read_kv("/proc/meminfo")
    used = m["MemTotal"] - m["MemAvailable"]
    return {
        "total_kb": m["MemTotal"],
        "used_kb": used,
        "free_kb": m["MemAvailable"],
    }

def net_bytes():
    rx = tx = 0
    with open("/proc/net/dev") as f:
        for line in f.readlines()[2:]:
            data = line.split()
            rx += int(data[1])
            tx += int(data[9])
    return rx, tx

def uptime():
    with open("/proc/uptime") as f:
        return int(float(f.readline().split()[0]))

def collect():
    rx, tx = net_bytes()
    return {
        "hostname": socket.gethostname(),
        "timestamp": int(time.time()),
        "status": "OK",
        "faults": 0,
        "cpu_percent": cpu_percent(),
        "loadavg": open("/proc/loadavg").read().strip(),
        "memory": mem_info(),
        "uptime_sec": uptime(),
        "net_rx_bytes": rx,
        "net_tx_bytes": tx,
    }

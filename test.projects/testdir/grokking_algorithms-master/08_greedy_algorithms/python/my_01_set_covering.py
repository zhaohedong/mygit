#!/usr/bin/python
states_needed = set(["mt", "wa", "or", "id", "nv", "ut", "ca", "az"])

stations = {}
stations["kone"] = set(["id", "nv", "ut"])
stations["ktwo"] = set(["wa", "id", "mt"])
stations["kthree"] = set(["or", "nv", "ca"])
stations["kfour"] = set(["nv", "ut"])
stations["kfive"] = set(["ca", "az"])

final_stations = set()

while states_needed:
    best_station = None
    best_covered = set() 
    current_covered = set() 
    for station,states in stations.items():
        current_covered = states_needed & states 
        if len(current_covered) > len(best_covered):
            best_covered = current_covered
            best_station = station
    final_stations.add(best_station)
    states_needed -= best_covered 
    print final_stations
print final_stations

        

#!/usr/bin/python3
import sys
import subprocess
import argparse

def run_eventSelector():
    cmd = "eventSelector"
    subprocess.run(cmd, shell=True)

def run_headtailAnalyzer(flag_stat, evtNum):
    if flag_stat:
        cmd = ["headtailAnalyzer", "-s", evtNum]
    else:
        cmd = ["headtailAnalyzer", evtNum]
    subprocess.run(cmd, shell=False)

def run_slopeVisualizer():
    cmd = "slopeVisualizer"
    subprocess.run(cmd, shell=True)

def main():
    if len(sys.argv) < 2:
        print("Usage: headtailAnalyzer.py [-s] [evtNum]")
        exit(1)

    parser = argparse.ArgumentParser(description="Run headtailAnalyzer")
    parser.add_argument("evtNum", help="Event number", nargs="*")
    parser.add_argument("-s", "--stat", help="Run headtailAnalyzer in stat mode", action="store_true")
    args = parser.parse_args()
    flag_stat = 0
    if args.stat:
        print("Running headtailAnalyzer in stat mode...")
        flag_stat = 1

    if flag_stat:
        print("Running eventSelector...")
        run_eventSelector()
        with open("selectedEvent.dat", "r") as f:
            event_data = f.readlines()[1:]
            event_data = [x.strip() for x in event_data]
        with open("slopeList.dat", "w") as f:
            f.write("# evtNum\txz\txz_error\tyz\tyz_error\n")
        print("calculating slope...")
        for count, event in enumerate(event_data):
            run_headtailAnalyzer(flag_stat, event)
            print("Processing : "+str(count)+"/"+str(len(event_data))+"\r", end="")
        run_slopeVisualizer()
    else:
        run_headtailAnalyzer(flag_stat, args.evtNum[0])

if __name__ == "__main__":
    main()
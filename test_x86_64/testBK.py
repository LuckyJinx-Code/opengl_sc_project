import json
import random
import subprocess
import time
import matplotlib.pyplot as plt
import math
import os
import sys
import psutil

valid_components = [
    "VSI", "CDI", "Airspeed", "Altimeter", "Compass", "SSI", "Waypoints",
    "WaypointInfo", "Random"]

valid_graph_types = ["Bar", "Logarithmic"]

valid_controller_modes = ["None", "Relaxed", "Extreme"]

# Function to create a JSON file with n components
def create_json(filename, n, component_name, controller_mode):
    controllers = []
    components = []

    params = ["posX", "posY", "posZ", "rotX", "rotY", "rotZ"]

    if controller_mode == "Relaxed":
        flight_radius = 300*random.random()
        flight_speed = (800 + random.random()*100)/3600
        flight_time = flight_radius/flight_speed

        posX_1 = -100000 + random.random() * 20000
        posX_2 = posX_1 + flight_radius

        posY_1 = 4000 + random.random() * 1000
        posY_2 = posY_1 + 1000

        posZ_1 = -100000 + random.random() * 20000
        posZ_2 = posZ_1 + flight_radius


        controllers = [
            {
                "parameter" : "posX",
                "easing" : "cosinusoid",
                "first_value" : posX_1,
                "last_value" : posX_2,
                "duration" : flight_time,
                "loop_mode" : "restart"
            },
            {
                "parameter" : "posY",
                "easing" : "sinusoid",
                "first_value" : posY_1,
                "last_value" : posY_2,
                "duration" : 100,
            },
            {
                "parameter" : "posZ",
                "easing" : "sinusoid",
                "first_value" : posZ_1,
                "last_value" : posZ_2,
                "duration" : flight_time,
                "loop_mode" : "restart"
            },
            {
                "parameter" : "rotY",
                "easing" : "linear",
                "first_value" : 0,
                "last_value" : math.pi,
                "duration" : flight_time,
                "loop_mode" : "restart"
            }
        ]

    if controller_mode == "Extreme":
        for param in params:
            controllers.append({
                "parameter" : param,
                "easing" : "sinusoid",
                "first_value" : -100000 + random.random()*200000,
                "last_value" : -100000 + random.random()*200000,
                "duration" : 0.5 + 2.5*random.random(),
                "loop_mode" : "restart"
            })

    for _ in range(n):
        component = {
            "component_name": 
              component_name if not component_name == "Random" 
              else random.choice(valid_components[:-1]),
            "posX": random.randint(-625, 625),
            "posY": random.randint(-400, 400)
        }
        components.append(component)

    data = {
        "controllers" : controllers,
        "components": components
    }
    
    # Write JSON data to file
    with open(filename, 'w') as json_file:
        json.dump(data, json_file, indent=4)

def monitor_cpu_usage(process, interval=0.5):
    """
    Monitors CPU usage of a process at regular intervals
    and calculates the average CPU usage.
    
    Args:
        process: The process to monitor.
        interval: Time interval between each CPU usage check (in seconds).
        
    Returns:
        avg_cpu: Average CPU usage during the process runtime.
    """
    cpu_usages = []

    try:
        # Continuously monitor CPU usage while process is running
        while process.poll() is None:  # poll() returns None if the process is still running
            cpu_percent = psutil.cpu_percent(interval=interval)  # Measure CPU usage
            cpu_usages.append(cpu_percent)
            time.sleep(interval)
    except psutil.NoSuchProcess:
        pass

    if cpu_usages:
        avg_cpu = sum(cpu_usages) / len(cpu_usages)
        return avg_cpu
    else:
        return 0.0

def execute_program_and_monitor(command):
    """
    Executes a program, captures its output, and monitors its CPU usage.
    
    Args:
        command: The command to execute the program (e.g., a list of command and arguments).
    
    Returns:
        output: The captured output of the program.
        avg_cpu_usage: The average CPU usage of the program.
    """
    try:
        # Start the process and capture its output (stdout)
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        # Store output and monitor CPU usage
        output = []
        avg_cpu_usage = monitor_cpu_usage(process)

        # Read the program's output line by line
        while True:
            line = process.stdout.readline()
            if not line and process.poll() is not None:  # Process is done
                break
            if line:  # If there is output
                output.append(line.strip())

        # Ensure all output is captured
        stdout, stderr = process.communicate()
        if stdout:
            output.append(stdout.strip())
        if stderr:
            output.append(stderr.strip())
        
        avg_fps = calculate_fps_average(output)

        return avg_fps, avg_cpu_usage
    
    except Exception as e:
        print(f'Error while executing the program: {e}')
        return None, None

def calculate_fps_average(fps_lines):
    fps_values = []
    
    # Loop through each line and extract the number after "FPS:"
    for line in fps_lines:
        splitted_line = line.split(":")
        if len(splitted_line) == 2:
            fps = splitted_line[1].strip()
            fps_values.append(float(fps))
    
    # Calculate the average FPS
    average_fps = sum(fps_values) / len(fps_values)
    
    return average_fps


component_counts = [1, 10, 50, 100, 500, 1000, 5000, 10000]
#component_counts = [1, 10]
#gpu_name = input("GPU name: ")

#while not component_name in valid_components:
#    component_name = input("Component: ")

#graph_type = ""

#while not graph_type in valid_graph_types:
#    graph_type = input("Graph type (Bar or Logarithmic): ")

#controller_mode = ""

#while not controller_mode in valid_controller_modes:
#    controller_mode = input("Controller mode (None, Relaxed, Extreme): ")

gpu_name = "AMD"
graph_type = "Logarithmic"
controller_mode = "Relaxed"

def gen_subplots(components):
    fig, plts = plt.subplots(3,2, figsize=(10, 10))
    fig.subplots_adjust(left=None, bottom=None, right=None, top=None, wspace=None, hspace=None)
    for i in range(3):
        avg_fps_results = []
        avg_cpu_results = []

        for count in component_counts:
            component_name = components[i]
            json_filename = f'components_{count}.json'
                
            # Step 1: Create JSON file
            create_json(json_filename, count, component_name, controller_mode)
                
            # Step 2: Run executable and get average
            command = ["./scs", json_filename , "10"]

            avg_fps, avg_cpu = execute_program_and_monitor(command)

            print(f'FPS: {avg_fps}')
            print(f'CPU: {avg_cpu}')

            avg_fps_results.append(avg_fps)
            avg_cpu_results.append(avg_cpu)


        fig.suptitle(f'{gpu_name} performance with {component_name} widgets')

        plts[i,0].set(xlabel=f'Number of {component_name} components', ylabel="FPS")
        if graph_type == "Logarithmic":
            plts[i,0].plot(component_counts, avg_fps_results, color='skyblue')
            plts[i,0].set_xscale('log')
            plts[i,0].set_ylim(ymin=0, ymax=60)
        else:
            plts[i,0].bar([str(ele) for ele in component_counts], avg_fps_results, color='skyblue')
        plts[i,0].set_title(f'FPS with {component_name} widgets', size=10)
        plts[i,0].grid(True, axis='y', linestyle='--', linewidth=0.7)

        #plt.show()

        plts[i,1].set(xlabel=f'Number of {component_name} components', ylabel="CPU (%)")
        if graph_type == "Logarithmic":
            plts[i,1].plot(component_counts, avg_cpu_results, color='red')
            plts[i,1].set_xscale('log')
            plts[i,1].set_ylim(ymin=0, ymax=100)
        else:
            plts[i,1].bar([str(ele) for ele in component_counts], avg_cpu_results, color='red')
        plts[i,1].set_title(f'CPU (%) with {component_name} widgets')
        plts[i,1].grid(True, axis='y', linestyle='--', linewidth=0.7)
    
    return fig

fig = gen_subplots(["Compass", "VSI", "Waypoints"])
fig.tight_layout()
fig.savefig(f'{gpu_name}.pdf', format="pdf", bbox_inches="tight")
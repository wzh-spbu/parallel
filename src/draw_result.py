import matplotlib.pyplot as plt
import os

file_path = "./speedup_results.txt"
threads = []
execution_time = []
speedup = []

with open(file_path, "r") as file:
    file.readline()
    for line in file:
        data = line.strip().split("\t")
        threads.append(int(data[0]))
        execution_time.append(float(data[1]))
        speedup.append(float(data[2]))


plt.figure(figsize=(8, 6))
plt.scatter(threads, execution_time, label="Execution Time", marker='x')
plt.plot(threads, execution_time, linestyle='--', alpha=0.7)
plt.xscale('log', base=2)
plt.xlabel("Threads (log scale)")
plt.ylabel("Execution Time (seconds)")
plt.title("Execution Time vs Threads")
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.show()

plt.figure(figsize=(8, 6))
plt.scatter(threads, speedup, label="Speedup", marker='o', color='r')
plt.plot(threads, speedup, linestyle='--', alpha=0.7, color='r')
plt.xscale('log', base=2)
plt.xlabel("Threads (log scale)")
plt.ylabel("Speedup")
plt.title("Speedup vs Threads")
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.show()
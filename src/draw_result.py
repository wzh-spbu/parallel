import matplotlib.pyplot as plt
import pandas as pd


filename = "./speedup_results.txt"
df = pd.read_csv(filename, sep="\t")


plt.figure(figsize=(12, 8))
for N in df["N"].unique():
    subset = df[df["N"] == N]
    plt.plot(subset["Threads"], subset["Speedup"], label=f"N={N}")
plt.title("Speedup vs Threads for Different Matrix Sizes", fontsize=16)
plt.xlabel("Threads", fontsize=14)
plt.ylabel("Speedup", fontsize=14)
plt.legend(title="Matrix Size (N)", fontsize=12)
plt.grid(True)
plt.show()


plt.figure(figsize=(12, 8))
for N in df["N"].unique():
    subset = df[df["N"] == N]
    plt.plot(subset["Threads"], subset["Execution Time"], label=f"N={N}")
plt.title("Execution Time vs Threads for Different Matrix Sizes", fontsize=16)
plt.xlabel("Threads", fontsize=14)
plt.ylabel("Execution Time (s)", fontsize=14)
plt.legend(title="Matrix Size (N)", fontsize=12)
plt.grid(True)
plt.show()

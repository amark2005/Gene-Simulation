import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

labels = df["label"].unique()
colors = {"INBRED": "red", "OUTBRED": "blue", "MIXED": "green"}

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

for label in labels:
    grp = df[df["label"] == label].sort_values("gen")
    x = grp["gen"].to_numpy()
    ax1.plot(x, grp["avgfitness"].to_numpy(), label=label, color=colors[label], linewidth=2)
    ax2.plot(x, grp["fixedloci"].to_numpy(), label=label, color=colors[label], linewidth=2)

ax1.set_title("Avg Fitness Over Generations")
ax1.set_xlabel("Generation")
ax1.set_ylabel("Avg Fitness")
ax1.legend()
ax1.grid(True)

ax2.set_title("Fixed Deleterious Loci")
ax2.set_xlabel("Generation")
ax2.set_ylabel("Fixed Loci Count")
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig("inbreeding_plot.png", dpi=150)
plt.show()
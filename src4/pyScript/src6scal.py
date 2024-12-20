import pandas as pd
import matplotlib.pyplot as plt

# Load the data into a DataFrame
data = {
    "Test Type": ["Strong"] * 25 + ["Weak"] * 5,
    "Problem Size": ["64x64", "64x64", "64x64", "64x64", "64x64", "128x128", "128x128", "128x128", "128x128", "128x128",
                     "256x256", "256x256", "256x256", "256x256", "256x256", "512x512", "512x512", "512x512", "512x512", "512x512",
                     "1024x1024", "1024x1024", "1024x1024", "1024x1024", "1024x1024", "64x64", "128x128", "256x256", "512x512", "1024x1024"],
    "Processors": [1, 2, 4, 8, 16] * 5 + [1, 2, 4, 8, 16],
    "Time (ms)": [2] * 30
}

df = pd.DataFrame(data)

# Plot all data in one figure
plt.figure(figsize=(10, 6))

# Strong Scaling
strong_df = df[df["Test Type"] == "Strong"]
for problem_size in strong_df["Problem Size"].unique():
    subset = strong_df[strong_df["Problem Size"] == problem_size]
    plt.plot(subset["Processors"], subset["Time (ms)"], marker='o', linestyle='-', label=f"Strong Scaling - {problem_size}")

# Weak Scaling
weak_df = df[df["Test Type"] == "Weak"]
plt.plot(weak_df["Processors"], weak_df["Time (ms)"], marker='o', linestyle='--', color='b', label="Weak Scaling")

# Set labels and title
plt.xlabel("Number of Processors")
plt.ylabel("Time (ms)")
plt.title("Strong and Weak Scaling Performance")
plt.xscale('log', base=2)
plt.yscale('log')
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.show()

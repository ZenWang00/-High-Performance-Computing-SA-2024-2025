import pandas as pd
import matplotlib.pyplot as plt

data = {
    'Scenario': ['1 Node Strong', '1 Node Weak', 'Multi Node Strong', 'Multi Node Weak'],
    'Processes': [
        [1, 2, 4, 8, 16, 20],  # 1 Node Strong
        [1, 2, 4, 8, 16, 20],  # 1 Node Weak
        [1, 2, 4, 8, 16],      # Multi Node Strong
        [1, 2, 4, 8, 16]       # Multi Node Weak
    ],
    'Times': [
        [0.121156, 0.061265, 0.031319, 0.017155, 0.010006, 0.007312],  # 1 Node Strong
        [0.121629, 0.121917, 0.125099, 0.130813, 0.143880, 0.146116],  # 1 Node Weak
        [0.121441, 0.061942, 0.035183, 0.024478, 0.022346],            # Multi Node Strong
        [0.121217, 0.122704, 0.124759, 0.127914, 0.133656]             # Multi Node Weak
    ]
}

df_list = []
for i, scenario in enumerate(data['Scenario']):
    temp_df = pd.DataFrame({
        'Processes': data['Processes'][i],
        'Time': data['Times'][i],
        'Scenario': scenario
    })
    df_list.append(temp_df)
df = pd.concat(df_list, ignore_index=True)

baseline_times = df[df['Processes'] == 1].set_index('Scenario')['Time']
df['Efficiency'] = df.apply(lambda row: baseline_times[row['Scenario']] / (row['Time'] * row['Processes']), axis=1)

fig, ax = plt.subplots(2, 2, figsize=(14, 10), sharey=True)
for i, axi in enumerate(ax.flat):
    scenario_data = df[df['Scenario'] == data['Scenario'][i]]
    axi.plot(scenario_data['Processes'], scenario_data['Time'], marker='o', label='Time')
    axi.plot(scenario_data['Processes'], scenario_data['Efficiency'], marker='x', linestyle='--', label='Efficiency')
    axi.set_title(data['Scenario'][i])
    axi.set_xlabel('Number of Processes')
    axi.set_ylabel('Time (s) / Efficiency')
    axi.legend()
    axi.grid(True)

plt.tight_layout()
plt.show()
